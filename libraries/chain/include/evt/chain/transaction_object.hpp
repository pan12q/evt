/**
 *  @file
 *  @copyright defined in evt/LICENSE.txt
 */
#pragma once
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include <evt/chain/transaction.hpp>
#include <evt/chain/multi_index_includes.hpp>

namespace evt { namespace chain {
using boost::multi_index_container;
using namespace boost::multi_index;
/**
 * The purpose of this object is to enable the detection of duplicate transactions. When a transaction is included
 * in a block a transaction_object is added. At the end of block processing all transaction_objects that have
 * expired can be removed from the index.
 */
class transaction_object : public chainbase::object<transaction_object_type, transaction_object> {
    OBJECT_CTOR(transaction_object)

    id_type             id;
    time_point_sec      expiration;
    transaction_id_type trx_id;
    uint32_t            block_num;
};

struct by_expiration;
struct by_trx_id;
using transaction_multi_index = chainbase::shared_multi_index_container<
    transaction_object,
    indexed_by<
        ordered_unique<tag<by_id>, BOOST_MULTI_INDEX_MEMBER(transaction_object, transaction_object::id_type, id)>,
        ordered_unique<tag<by_trx_id>, BOOST_MULTI_INDEX_MEMBER(transaction_object, transaction_id_type, trx_id)>,
        ordered_unique<
            tag<by_expiration>,
            composite_key<transaction_object, BOOST_MULTI_INDEX_MEMBER(transaction_object, time_point_sec, expiration),
                          BOOST_MULTI_INDEX_MEMBER(transaction_object, transaction_object::id_type, id)>>>>;

typedef chainbase::generic_index<transaction_multi_index> transaction_index;

}}  // namespace evt::chain

CHAINBASE_SET_INDEX_TYPE(evt::chain::transaction_object, evt::chain::transaction_multi_index);
FC_REFLECT(evt::chain::transaction_object, (expiration)(trx_id)(block_num));
