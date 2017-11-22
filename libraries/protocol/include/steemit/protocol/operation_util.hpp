
#pragma once

#include <steemit/protocol/authority.hpp>

#include <fc/variant.hpp>

#include <boost/container/flat_set.hpp>

#include <string>
#include <vector>

//
// Place DECLARE_OPERATION_TYPE in a .hpp file to declare
// functions related to your operation type
//
#define DECLARE_OPERATION_TYPE( OperationType )                                  \
namespace fc {                                                                   \
                                                                                 \
void to_variant( const OperationType&, fc::variant& );                           \
void from_variant( const fc::variant&, OperationType& );                         \
                                                                                 \
} /* fc */                                                                       \
                                                                                 \
namespace steemit { namespace protocol {                                         \
                                                                                 \
void operation_validate( const OperationType& o );                               \
void operation_get_required_authorities( const OperationType& op,                \
                                         flat_set< account_name_type >& active,  \
                                         flat_set< account_name_type >& owner,   \
                                         flat_set< account_name_type >& posting, \
                                         vector< authority >& other );           \
                                                                                 \
} } /* steemit::protocol */

namespace steemit { namespace protocol {

   struct get_required_authorities_visitor
   {
      typedef void result_type;

      flat_set< account_name_type >&   active;
      flat_set< account_name_type >&   owner;
      flat_set< account_name_type >&   posting;
      std::vector< authority >&        other;

      get_required_authorities_visitor(
            flat_set< account_name_type >& a,
            flat_set< account_name_type >& own,
            flat_set< account_name_type >& post,
            std::vector< authority >& oth )
         : active( a ), owner( own ), posting( post ), other( oth ) {}

      // If it is an operation type, we need to unwrap it and call the visitor again.
      template< typename ... Ts >
      void operator()( const static_variant< Ts... >& v )
      {
         v.visit( get_required_authorities_visitor( active, owner, posting, other ) );
      }

      template< typename T >
      void operator()( const T& v )const
      {
         v.get_required_active_authorities( active );
         v.get_required_owner_authorities( owner );
         v.get_required_posting_authorities( posting );
         v.get_required_authorities( other );
      }
   };

} } /* steemit::protocol */
