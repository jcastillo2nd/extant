/** @file include/extant/set/common.h
 * Common xtnt_node and xtnt_node_set definitions
 * The structures and defines used to declare the xtnt_node_set interfaces
 * and functions.  
 */

/** @def XTNT_NODE_HEAD
 * Index for head node link
 */

/** @def XTNT_NODE_LEFT
 * Index for left node link 
 */

/** @def XTNT_NODE_MIDDLE
 * Index for middle node link
 */

/** @def XTNT_NODE_CENTER
 * Index for center node link
 */

/** @def XTNT_NODE_PARENT
 * Index for parent node link
 */

/** @def XTNT_NODE_TAIL
 * Index for tail node link
 */

/** @def XTNT_NODE_RIGHT
 * Index for right node link
 */

/** @struct xtnt_node include/extant/set/common.h <extant/set/common.h>
 * Node element for Node Set operations.
 * All operations on nodes require a reference to an element of this struct.
 * @note Attributes marked public can be accessed by any thread. All other attributes require a lock.
 *
 * @property void* xtnt_node::value
 * @public
 * Pointer to any value the node holds. 
 * @attention This property should be treated as immutable.
 *
 * @var struct xtnt_node* xtnt_node::link[3]
 * @private
 * Array of pointers to relative nodes.
 * @warning The pointers are managed by the set operations and may or may not
 * have valid values.
 *
 * @var xtnt_uint_t xtnt_node::key
 * @private
 * An identifier for the node
 *
 * @var xtnt_uint_t xtnt_node::state
 * @private
 * The state of the node after operations
 *
 * @var xtnt_uint_t xtnt_node::quirk
 * @public
 * An attribute of the node.
 * @attention This property should be treated as immutable.
 * @note This may be used in place of a namespace, or for qualifying the node
 * as a particular type when referencing `xtnt_node::value`.
 *
 * @var pthread_mutex_t xtnt_node::lock
 * @public
 * Lock used to operate on any members
 */

/** @struct xtnt_node_set_state include/extant/set/common.h <extant/set/common.h>
 * Node Set state
 *
 * @var xtnt_uint_t xtnt_node_set_state::size
 * @private
 * The number of elements in the Node Set capacity
 *
 * @var xtnt_uint_t xtnt_node_set_state::count
 * @private
 * The number of elements currently within the Node Set
 *
 * @var xtnt_uint_t xtnt_node_set::err
 * @private
 * The error for the state of the Node Set
 */

/** @struct xtnt_node_set include/extant/set/common.h <extant/set/common.h>
 * Node Set interface
 *
 * @var const struct xtnt_node_set_if xtnt_node_set::fn
 * @private
 * The function pointers for operation on the Node Set
 *
 * @var struct xtnt_node* xtnt_node_set::link
 * @private
 * The Node element pointers for the members of the Node Set
 *
 * @var struct xtnt_node_set_state xtnt_node_set::state
 * @private
 * The members that indicate the status of the Node Set
 *
 * @var pthread_mutex_t xtnt_node_set::lock
 * @public
 * The lock used when accessing state or modifying the Node Set
 */

/** @struct xtnt_node_set_if
 * Function interface for Node Set
 *
 * @fn xtnt_status_t (*index)(struct xtnt_node_set *set, xtnt_int_t index, struct xtnt_node **found)
 * Return node at index of Node Set
 * @relates xtnt_node_set_if
 * @note For non-indexed Node Sets ( e.g. trees ) this may operate the same as search.
 * @param set A Node Set to operate on
 * @param index An index to reference for Node of Node Set
 * @param found[out] The pointer to store the found Node at, or NULL if not found
 * @return status result of operation
 *
 * @fn xtnt_status_t (*search)(struct xtnt_node_set *set, xtnt_uint_t key, struct xtnt_node **found)
 * Search Node Set for Node with matching key
 * @relates xtnt_node_set_if
 * @param set A Node Set to operate on
 * @param key The key to compare Nodes against
 * @param found[out] The pointer to store the found Node at, or NULL if not found
 * @return status result of operation
 *
 * @fn xtnt_status_t (*search_fn)(struct xtnt_node_set *set, void *fn, void *needle, struct xtnt_node **found)
 * Search Node Set for Node with search function fn, that accepts needle criteria
 * @relates xtnt_node_set_if
 * @param set A Node Set to operate on
 * @param fn A search function to run on each node
 * @param needle Search criteria for search function fn
 * @param found[out] The pointer to store the found Node at, or NULL if not found
 * @return status result of operation
 *
 * @fn xtnt_status_t (*first)(struct xtnt_node_set *set, struct xtnt_node **first)
 * Return first Node of Node Set
 * @relates xtnt_node_set_if
 * @note Definition varies based on Node Set implementation, e.g. Tree may return lowest key Node
 * @param set A Node Set to operate on
 * @param first[out] The pointer to store the Node at, or NULL on empty Node Set
 * @return status result of operation
 *
 * @fn xtnt_status_t (*last)(struct xtnt_node_set *set, struct xtnt_node **last)
 * Return last Node of Node Set
 * @relates xtnt_node_set_if
 * @note Definition varies based on Node Set implementation, e.g. Tree may return highest key Node
 * @param set A Node Set to operate on
 * @param last[out] The pointer to store the Node at, or NULL on empty Node Set
 * @return status result of operation
 *
 * @fn xtnt_status_t (*peek)(struct xtnt_node_set *set, struct xtnt_node **peek)
 * Return reference to next Node in Node Set
 * @relates xtnt_node_set_if
 * @param set A Node Set to operate on
 * @param peek[out] The pointer to store the Node at, or NULL on empty Node Set
 * @return status result of operation
 *
 * @fn xtnt_status_t (*root)(struct xtnt_node_set *set, struct xtnt_node **root)
 * Return the reference to the root Node in Node Set
 * @relates xtnt_node_set_if
 * @param set The Node Set to operate on
 * @param root[out] The pointer to store the Node at, or NULL on empty Node Set
 * @return status result of operation
 *
 * @fn xtnt_status_t (*state)(struct xtnt_node_set *set, struct xtnt_node_set_state **state)
 * Return the state of the Node Set
 * @relates xtnt_node_set_if
 * @param set The Node Set to operate on
 * @param state[out] The pointer to store the Node State at
 * @return status result of operation
 *
 * @fn xtnt_status_t (*insert)(struct xtnt_node_set *set, struct xtnt_node *node)
 * Insert Node into Node Set
 * @relates xtnt_node_set_if
 * @param set The Node Set to operate on
 * @param node The Node to insert
 * @return status result of operation
 *
 * @fn xtnt_status_t (*insert_at)(struct xtnt_node_set *set, xtnt_uint_t index, struct xtnt_node *node, struct xtnt_node **replaced)
 * Insert Node into Node Set at index
 * @relates xtnt_node_set_if
 * @param set The Node Set to operate on
 * @param index The index to insert at
 * @param node the The Node to insert
 * @param[out] The pointer to store the Node that was replaced
 * @return status result of operation
 *
 * @fn xtnt_status_t (*push)(struct xtnt_node_set *set, struct xtnt_node *node)
 * Insert Node into next position in Node Set
 * @relates xtnt_node_set_if
 * @param set The Node Set to operate on
 * @param node The node to insert
 * @return status result of operation
 *
 * @fn xtnt_status_t (*remove)(struct xtnt_node_set *set, struct xtnt_node *node)
 * Remove a node from a Node Set
 * @relates xtnt_node_set_if
 * @param set The Node Set to operate on
 * @param node The Node to remove from Node Set
 * @return status result of operation
 */
    xtnt_status_t (*remove_at)(struct xtnt_node_set_if *set, xtnt_uint_t index, struct xtnt_node **removed);
    xtnt_status_t (*pop)(struct xtnt_node_set_if *set, struct xtnt_node *node);

    xtnt_status_t (*sort)(struct xtnt_node_set_if *set);
    xtnt_status_t (*sort_reverse)(struct xtnt_node_set_if *set);
    xtnt_status_t (*sort_fn)(struct xtnt_node_set_if *set, void *fn);

    xtnt_status_t (*grow)(struct xtnt_node_set_if *set);
    xtnt_status_t (*grow_eval)(struct xtnt_node_set_if *set, size_t eval);
    xtnt_status_t (*shrink)(struct xtnt_node_set_if *set);
    xtnt_status_t (*shrink_eval)(struct xtnt_node_set_if *set, size_t eval);
};
