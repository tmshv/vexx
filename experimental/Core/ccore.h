#ifndef CCORE_H
#define CCORE_H

#define C_FOREACH(a, c) for( c::iterator it = c.begin(); it != c.end(); ++it, c=*it )

#define foreach(assign, container) C_FOREACH(assign, container)

#define C_DISABLE_COPY(c) private: c(const c &); c & operator=(const c &);

inline void CNoop()
  {
  }

#endif // CCORE_H
