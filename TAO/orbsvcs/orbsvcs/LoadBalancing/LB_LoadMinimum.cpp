#include "LB_LoadMinimum.h"
#include "ace/OS_NS_sys_time.h"
#include "LB_LoadMap.h"
#include "LB_Random.h"

#include "orbsvcs/PortableGroup/PG_conf.h"

#include "tao/debug.h"
#include "tao/ORB_Constants.h"

#include "ace/Null_Mutex.h"
#include "ace/OS_NS_string.h"


ACE_RCSID (LoadBalancing,
           LB_LoadMinimum,
           "$Id$")


#if !defined (__ACE_INLINE__)
#include "LB_LoadMinimum.inl"
#endif /* defined INLINE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_LB_LoadMinimum::TAO_LB_LoadMinimum (PortableServer::POA_ptr poa)
  : poa_ (PortableServer::POA::_duplicate (poa)),
    load_map_ (0),
    lock_ (0),
    properties_ (),
    tolerance_ (TAO_LB::LM_DEFAULT_TOLERANCE),
    dampening_ (TAO_LB::LM_DEFAULT_DAMPENING),
    per_balance_load_ (TAO_LB::LM_DEFAULT_DAMPENING)
{
  // A load map that retains previous load values at a given location
  // and lock are only needed if dampening is enabled, i.e. non-zero.
  if (this->dampening_ != 0)
    {
      ACE_NEW (this->load_map_, TAO_LB_LoadMap (TAO_PG_MAX_LOCATIONS));

      ACE_NEW (this->lock_, TAO_SYNCH_MUTEX);
    }

  // Initialize the random load balancing strategy.
  TAO_LB_Random::init ();
}

TAO_LB_LoadMinimum::~TAO_LB_LoadMinimum (void)
{
  delete this->load_map_;
  delete this->lock_;
}

char *
TAO_LB_LoadMinimum::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("LoadMinimum");
}

CosLoadBalancing::Properties *
TAO_LB_LoadMinimum::get_properties (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CosLoadBalancing::Properties * props = 0;
  ACE_NEW_THROW_EX (props,
                    CosLoadBalancing::Properties (this->properties_),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  return props;
}

void
TAO_LB_LoadMinimum::push_loads (
    const PortableGroup::Location & the_location,
    const CosLoadBalancing::LoadList & loads
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Only the first load is used by this load balancing strategy.
  if (loads.length () == 0)
    ACE_THROW (CORBA::BAD_PARAM ());

  CosLoadBalancing::Load load;  // Unused

  this->push_loads (the_location,
                    loads,
                    load
                    ACE_ENV_ARG_PARAMETER);
}

void
TAO_LB_LoadMinimum::push_loads (
    const PortableGroup::Location & the_location,
    const CosLoadBalancing::LoadList & loads,
    CosLoadBalancing::Load & load
    ACE_ENV_ARG_DECL)
{
  if (loads.length () == 0)
    ACE_THROW (CORBA::BAD_PARAM ());

  // Only the first load is used by this load balancing strategy.
  const CosLoadBalancing::Load & new_load = loads[0];

  if (this->load_map_ != 0)
    {
      ACE_GUARD (TAO_SYNCH_MUTEX, guard, *this->lock_);

      TAO_LB_LoadMap::ENTRY * entry;
      if (this->load_map_->find (the_location, entry) == 0)
        {
          CosLoadBalancing::Load & previous_load = entry->int_id_;

          if (previous_load.id != new_load.id)
            ACE_THROW (CORBA::BAD_PARAM ());  // Somebody switched
                                              // LoadIds on us!

          previous_load.value =
            this->effective_load (previous_load.value, new_load.value);

          load = previous_load;
        }
      else
        {
          const CosLoadBalancing::Load eff_load =
            {
              new_load.id,
              this->effective_load (0, new_load.value)
            };

          if (this->load_map_->bind (the_location, eff_load) != 0)
            {
              if (TAO_debug_level > 0)
                ACE_ERROR ((LM_ERROR,
                            "ERROR: TAO_LB_LoadMinimum - "
                            "Unable to push loads\n"));

              ACE_THROW (CORBA::INTERNAL ());
            }

          load = eff_load;
        }
    }
  else
    {
      load.id = new_load.id;
      load.value = this->effective_load (0, new_load.value);
    }
}

CosLoadBalancing::LoadList *
TAO_LB_LoadMinimum::get_loads (CosLoadBalancing::LoadManager_ptr load_manager,
                               const PortableGroup::Location & the_location
                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosLoadBalancing::LocationNotFound))
{
  if (CORBA::is_nil (load_manager))
    ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);

  CosLoadBalancing::LoadList_var loads =
    load_manager->get_loads (the_location
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  this->push_loads (the_location,
                    loads.in (),
                    loads[0]
                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return loads._retn ();
}


CORBA::Object_ptr
TAO_LB_LoadMinimum::next_member (
    PortableGroup::ObjectGroup_ptr object_group,
    CosLoadBalancing::LoadManager_ptr load_manager
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound,
                   PortableGroup::MemberNotFound))
{
  if (CORBA::is_nil (load_manager))
    ACE_THROW_RETURN (CORBA::BAD_PARAM (),
                      CORBA::Object::_nil ());

  PortableGroup::Locations_var locations =
    load_manager->locations_of_members (object_group
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  if (locations->length () == 0)
    ACE_THROW_RETURN (CORBA::TRANSIENT (),
                      CORBA::Object::_nil ());

  // @@ RACE CONDITION.  OBJECT GROUP MEMBERSHIP MAY CHANGE AFTER
  //    RETRIEVING LOCATIONS!  HOW DO WE HANDLE THAT?

  PortableGroup::Location location;
  CORBA::Boolean found_location =
    this->get_location (load_manager,
                        locations.in (),
                        location
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  if (found_location)
    {
/*
       ACE_DEBUG ((LM_DEBUG,
                   "RETURNING REFERENCE FOR LOCATION \"%s\"\n",
                   location[0].id.in ()));
*/

      return load_manager->get_member_ref (object_group,
                                           location
                                           ACE_ENV_ARG_PARAMETER);
    }
  else
    {
      // No loads have been reported for any of the locations the
      // object group members reside at.  If no loads have been
      // reported to the LoadManager, adaptive load balancing
      // decisions cannot be made.  Fall back on a non-adaptive
      // strategy, such as the Random load balancing strategy,
      // instead.
      //
      // @note The Random load balancing strategy is used since it is
      //       very lightweight and stateless.
/*
       ACE_DEBUG ((LM_DEBUG,
                   "CALLING RANDOM  REFERENCE FOR LOCATION \n"));
*/

      return TAO_LB_Random::_tao_next_member (object_group,
                                              load_manager,
                                              locations.in ()
                                              ACE_ENV_ARG_PARAMETER);
    }
}

void
TAO_LB_LoadMinimum::analyze_loads (
    PortableGroup::ObjectGroup_ptr object_group,
    CosLoadBalancing::LoadManager_ptr load_manager
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
    if (CORBA::is_nil (load_manager))
    ACE_THROW (CORBA::BAD_PARAM ());

  PortableGroup::Locations_var locations =
    load_manager->locations_of_members (object_group
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (locations->length () == 0)
    ACE_THROW (CORBA::TRANSIENT ());

  const CORBA::ULong len = locations->length ();

  CosLoadBalancing::Load total_load = { 0, 0 };
  CosLoadBalancing::Load avg_load = { 0, 0 };

  CORBA::Float min_load = FLT_MAX;

  CosLoadBalancing::LoadList tmp (len);
  tmp.length (1);

  // Iterate through the entire location list to determine
  // the average load of all the locations
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      ACE_TRY
        {
          const PortableGroup::Location & loc = locations[i];

          // Retrieve the load list for the location from the
          // LoadManager and push it to this Strategy's load
          // processor.
          CosLoadBalancing::LoadList_var current_loads =
            load_manager->get_loads (loc
                                     ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CosLoadBalancing::Load load;
          this->push_loads (loc,
                            current_loads.in (),
                            load
                            ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          total_load.value = total_load.value + load.value;
          tmp[i] = load;

          if ((load.value < min_load) && (load.value != 0))
          {
            min_load = load.value;
          }

          /*
          ACE_DEBUG ((LM_DEBUG,
                       "TOTAL == %f\n",
                       total_load.value));
          */

        }
      ACE_CATCH (CosLoadBalancing::LocationNotFound, ex)
        {
          // no location found
          //
        }
      ACE_ENDTRY;
      ACE_CHECK;
    }

  avg_load.value = total_load.value / len;
/*
  ACE_DEBUG ((LM_DEBUG,
                      "AVERAGE LOAD == %f"
                      "\tMIN LOAD == %f\n",
                      avg_load.value,
                      min_load));
*/

  // Iterate through the entire location list to determine
  // the location where the load has to be shed.
  for (CORBA::ULong j = 0; j < len; ++j)
    {
      ACE_TRY_EX (SECOND)
        {
          const PortableGroup::Location & loc = locations[j];

         /*
          ACE_DEBUG ((LM_DEBUG,
                       "EFFECTIVE_LOAD == %f\n"
                       "AVERAGE       == %f\n",
                       tmp[i].value,
                       avg_load.value));
        */

          if (tmp[j].value <= avg_load.value)
            {
              load_manager->disable_alert (loc
                                           ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK_EX (SECOND);
            }
          else
            {

              CORBA::Float percent_diff =
                (tmp[j].value / min_load) - 1;

              if (tmp[j].value == min_load)
              {
                percent_diff = 0;
              }
/*
          ACE_DEBUG ((LM_DEBUG,
                      "ALERT LOC == %u"
                      "\tMIN LOAD == %f\n"
                      "\tLOAD == %f\n"
                      "\tPERCENT == %f\n",
                      i,
                      min_load,
                      tmp[i].value,
                      percent_diff));
*/

              if (percent_diff <= TAO_LB::LM_DEFAULT_DIFF_AVERAGE_CUTOFF)
              {
                load_manager->disable_alert (loc
                                             ACE_ENV_ARG_PARAMETER);
                ACE_TRY_CHECK_EX (SECOND);
              }
              else if ((percent_diff > TAO_LB::LM_DEFAULT_DIFF_AVERAGE_CUTOFF)
                       && (percent_diff < 1))
              {
/*
                ACE_DEBUG ((LM_DEBUG,
                            "%P --- ALERTING LOCATION %u\n",
                            i));
*/
                load_manager->enable_alert (loc
                                            ACE_ENV_ARG_PARAMETER);
                ACE_TRY_CHECK_EX (SECOND);
              }
            }
        }
      ACE_CATCH (CosLoadBalancing::LocationNotFound, ex)
        {
          // no location found
          //
        }
      ACE_ENDTRY;
      ACE_CHECK;
    }

}

PortableServer::POA_ptr
TAO_LB_LoadMinimum::_default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

CORBA::Boolean
TAO_LB_LoadMinimum::get_location (
  CosLoadBalancing::LoadManager_ptr load_manager,
  const PortableGroup::Locations & locations,
  PortableGroup::Location & location
  ACE_ENV_ARG_DECL)
{
  CORBA::Float min_load = FLT_MAX;  // Start out with the largest
                                    // positive value.

  CORBA::ULong location_index = 0;
  CORBA::Boolean found_location = 0;
  CORBA::Boolean found_load = 0;

  const CORBA::ULong len = locations.length ();

  // Iterate through the entire location list to find the least loaded
  // of them.
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      ACE_TRY
        {
          const PortableGroup::Location & loc = locations[i];

          // Retrieve the load list for the location from the LoadManager
          // and push it to this Strategy's load processor.
          CosLoadBalancing::LoadList_var current_loads =
            load_manager->get_loads (loc
                                     ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          found_load = 1;

          CosLoadBalancing::Load load;
          this->push_loads (loc,
                            current_loads.in (),
                            load
                            ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
/*
          ACE_DEBUG ((LM_DEBUG,
                      "LOC == %u"
                      "\tMIN LOAD == %f\n"
                      "\tLOAD == %f\n",
                      i,
                      min_load,
                      load.value));
*/
          if (load.value < min_load)
            {

              if (i > 0 && load.value != 0)
                {
                  /*
                    percent difference =
                      (min_load - load.value) / load.value
                      == (min_load / load.value) - 1

                      The latter form is used to avoid a potential
                      arithmetic overflow problem, such as when
                      (min_load - load.value) > FLT_MAX, assuming that
                      either load.value is negative and min_load is
                      positive, or vice versa.
                  */
                  const CORBA::Float percent_diff =
                    (min_load / load.value) - 1;
                  /*
                    A "thundering herd" phenomenon may occur when
                    location loads are basically the same (e.g. only
                    differ by a very small amount), where one object
                    group member ends up receiving the majority of
                    requests from different clients.  In order to
                    prevent a single object group member from
                    receiving such request bursts, one of two equally
                    loaded locations is chosen at random.  Thanks to
                    Carlos, Marina and Jody at ATD for coming up with
                    this solution to this form of the thundering herd
                    problem.

                    See the documentation for
                    TAO_LB::LL_DEFAULT_LOAD_PERCENT_DIFF_CUTOFF in
                    LB_LoadMinimum.h for additional information.
                  */
                  if (percent_diff <= TAO_LB::LM_DEFAULT_LOAD_PERCENT_DIFF_CUTOFF)
                    {
                      // Prevent integer arithmetic overflow.
                      const CORBA::Float NUM_MEMBERS = 2;

                      // n == 0:  Use previously selected location.
                      // n == 1:  Use current location.
                      const CORBA::ULong n =
                        static_cast<CORBA::ULong> (NUM_MEMBERS * ACE_OS::rand ()
                                         / (RAND_MAX + 1.0));

                      ACE_ASSERT (n == 0 || n == 1);

                      if (n == 1)
                        {
                          min_load = load.value;
                          location_index = i;
                          found_location = 1;

//                           ACE_DEBUG ((LM_DEBUG,
//                                       "** NEW MIN_LOAD == %f\n",
//                                       min_load));
                        }

//                       if (n == 0)
//                         ACE_DEBUG ((LM_DEBUG, "^^^^^ PREVIOUS LOCATION\n"));
//                       else
//                         ACE_DEBUG ((LM_DEBUG, "^^^^^ CURRENT LOCATION\n"));

                    }
                  else
                    {
                      min_load = load.value;
                      location_index = i;
                      found_location = 1;

//                       ACE_DEBUG ((LM_DEBUG,
//                                   "***** NEW MIN_LOAD == %f\n",
//                                   min_load));
                    }
                }
              else
                {
                  min_load = load.value;
                  location_index = i;
                  found_location = 1;

//                   ACE_DEBUG ((LM_DEBUG,
//                               "NEW MIN_LOAD == %f\n",
//                               min_load));
                }
            }

          // ACE_DEBUG ((LM_DEBUG, "NEW MIN_LOAD == %f\n", min_load));
        }
      ACE_CATCH (CosLoadBalancing::LocationNotFound, ex)
        {
          // No load available for the requested location.  Try the
          // next location.
        }
      ACE_ENDTRY;
      ACE_CHECK_RETURN (0);
    }

//   ACE_DEBUG ((LM_DEBUG,
//               "FOUND_LOAD     == %u\n"
//               "FOUND_LOCATION == %u\n",
//               found_load,
//               found_location));

  // If no loads were found, return without an exception to allow this
  // strategy to select a member using an alternative method
  // (e.g. random selection).
  if (found_load)
    {
      if (found_location)
      {
        location = locations[location_index];
        //ACE_DEBUG ((LM_DEBUG, "LOCATED = %u\n", location_index));
      }
    }

   //ACE_DEBUG ((LM_DEBUG, "LOCATED = %u\n", location_index));

  return found_location;
}

void
TAO_LB_LoadMinimum::init (const PortableGroup::Properties & props
                          ACE_ENV_ARG_DECL)
{
  CORBA::Float tolerance = TAO_LB::LM_DEFAULT_TOLERANCE;
  CORBA::Float dampening = TAO_LB::LM_DEFAULT_DAMPENING;
  CORBA::Float per_balance_load = TAO_LB::LM_DEFAULT_PER_BALANCE_LOAD;

  const CORBA::ULong len = props.length ();
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      const PortableGroup::Property & property = props[i];
      if (ACE_OS::strcmp (property.nam[0].id.in (),
                          "org.omg.CosLoadBalancing.Strategy.LoadMinimum.Tolerance") == 0)
        {
          this->extract_float_property (property,
                                        tolerance
                                        ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          // Valid tolerance values are greater than or equal to one.
          if (tolerance < 1)
            ACE_THROW (PortableGroup::InvalidProperty (property.nam,
                                                       property.val));
        }

      else if (ACE_OS::strcmp (property.nam[0].id.in (),
                               "org.omg.CosLoadBalancing.Strategy.LoadMinimum.Dampening") == 0)
        {
          this->extract_float_property (property,
                                        dampening
                                        ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          // Dampening range is [0,1).
          if (dampening < 0 || dampening >= 1)
            ACE_THROW (PortableGroup::InvalidProperty (property.nam,
                                                       property.val));
        }

      else if (ACE_OS::strcmp (property.nam[0].id.in (),
                               "org.omg.CosLoadBalancing.Strategy.LoadMinimum.PerBalanceLoad") == 0)
        {
          this->extract_float_property (property,
                                        per_balance_load
                                        ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
    }

  this->properties_ = props;

  this->tolerance_          = tolerance;
  this->dampening_          = dampening;
  this->per_balance_load_   = per_balance_load;

  /*
   ACE_DEBUG ((LM_DEBUG,
               "--------------------------------\n"
               "tolerance          = %f\n"
               "dampening          = %f\n"
               "per_balance_load   = %f\n"
               "--------------------------------\n",
               tolerance,
               dampening,
               per_balance_load));
  */
}

void
TAO_LB_LoadMinimum::extract_float_property (
  const PortableGroup::Property & property,
  CORBA::Float & value
  ACE_ENV_ARG_DECL)
{
  if (!(property.val >>= value))
    ACE_THROW (PortableGroup::InvalidProperty (property.nam,
                                               property.val));
}

TAO_END_VERSIONED_NAMESPACE_DECL
