typedef unsigned long size_t;

template <class T> struct _plsm_linked_list {
  struct _plsm_linked_list<T> *next;
  T data;
};
template <class T> using plsm_linked_list = struct _plsm_linked_list<T>;

template <class T> struct _plsm_list_table_node {
  plsm_linked_list<T> *first, *last;
};
template <class T> using plsm_list_table_node = struct _plsm_list_table_node<T>;

template <class T> struct _plsm_list {
  plsm_list_table_node<T> **table;
  size_t size, table_size;
};
template <class T> using plsm_list = struct _plsm_list<T>;

#include <cstdlib>

template <class T>
inline auto create_plsm_linked_list(T data, plsm_linked_list<T> *next) {
  typedef plsm_linked_list<T> t;
  t *result = (t *)std::malloc(sizeof(t));
  result->data = data;
  result->next = next;
  return result;
}

template <class T>
inline auto create_plsm_list_table_node(plsm_linked_list<T> *first,
                                        plsm_linked_list<T> *last) {
  typedef plsm_list_table_node<T> t;
  t *result = (t *)std::malloc(sizeof(t));
  result->first = first;
  result->last = last;
  return result;
}

template <class T> inline void plsm_list_append(plsm_list<T> *list, T data) {
  size_t table_index = list->size % list->table_size;
  plsm_list_table_node<T> *table_node = list->table[table_index];

  plsm_linked_list<T> *node = create_plsm_linked_list<T>(data, nullptr);
  if (!table_node->last) {
    table_node->first = table_node->last = node;
  } else {
    table_node->last->next = node;
    table_node->last = node;
  }
  list->size += 1;
}

template <class T>
inline void plsm_list_set(plsm_list<T> *list, size_t index, T data) {
  if (index >= list->size) {
    std::exit(80);
  }

  size_t table_index = index % list->table_size;
  plsm_list_table_node<T> *table_node = list->table[table_index];

  size_t tmp = table_index;
  plsm_linked_list<T> *node = table_node->first;
  while (tmp != index) {
    node = node->next;
    tmp += table_index;
  }

  node->data = data;
}

template <class T>
inline T plsm_list_get(plsm_list<T> *list, size_t index) {
  if (index >= list->size) {
    std::exit(81);
  }

  size_t table_index = index % list->table_size;
  plsm_list_table_node<T> *table_node = list->table[table_index];

  size_t tmp = table_index;
  plsm_linked_list<T> *node = table_node->first;
  while (tmp != index) {
    node = node->next;
    tmp += list->table_size;
  }

  return node->data;
}

template <class T> inline plsm_list<T> create_plsm_list() {
  plsm_list<T> result;
  result.size = 0;
  result.table_size = 512; //1048576;

  typedef plsm_list_table_node<T> *tn_ptr;
  result.table = (tn_ptr *)std::calloc(result.table_size, sizeof(tn_ptr));

  for (size_t i = 0; i < result.table_size; i++) {
    result.table[i] = create_plsm_list_table_node<T>(nullptr, nullptr);
  }

  return result;
}
