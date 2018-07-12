/*
 * List.h
 *
 *  Created on: 31 mar 2015
 *      Author: jof
 */

#ifndef LIST_H_
#define LIST_H_

/**
 * Simple linked list
 * Type must have a public property next that is a pointer to a the same type
 */
template <typename T>
class List {
protected:
  T *m_first;
  T *m_last;
  T *m_current;
public:
  List() : m_first(nullptr), m_last(nullptr), m_current(nullptr) { };
  virtual ~List() {}

  void addItem(T *item)
  {
    if (m_first == nullptr) {
      // insert first element
      m_first = m_last = item;
    } else {
      // insert second or more elements
      m_last->next = item;
      m_last = item;
    }
  }

  T *popItem()
  {
      if (m_last == nullptr)
          return nullptr;
      if (m_first == nullptr)
          return nullptr;

      // find previous item
      T *prevItm = m_first;
      while (prevItm != nullptr) {
          if (prevItm->next == m_last) {
              break;
          }
          prevItm = prevItm->next;
      }

      // backup current pointer
      if (m_current == m_last)
          m_current = prevItm;

      // pop off list and return poped item
      T *popItm = m_last;
      m_last = prevItm;
      return popItm;
  }

  /**
   * Gets the first item and resets to internal iterator
   */
  T *first()
  {
    m_current = m_first;
    return m_first;
  }

  inline T *current() { return m_current; }

  /**
   * Returns the next item in list, is nullptr if its the last
   * Forwards the iterator pointer by one
   */
  T *next()
  {
    T *tmp = m_current->next;
    if (tmp != nullptr) {
      m_current = tmp;
    }
    return tmp;
  }

  T *last()
  {
    return m_last;
  }

  /**
   * returns the size of the list
   * NOTE ! might be costly is O(N) function
   */
  int length()
  {
    int len = 0;
    for (T *itm = m_first; itm != nullptr; itm = itm->next) {
        ++len;
    }
    return len;
  }
};



#endif /* LIST_H_ */
