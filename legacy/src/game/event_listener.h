#ifndef RUNESIM_EVENT_LISTENER_H
#define RUNESIM_EVENT_LISTENER_H

#include "event.h"

#define MAX_DATA_SIZE 8
class EventListener;
class EventListenerImpl {
  friend EventListener;
private:
  RSID listenerId;
  EventType type;
  std::function<void(RSID, Event)> func;
  i64 data[MAX_DATA_SIZE];
};

class EventListener {
private:
  sptr<EventListenerImpl> content;
public:
  i64 *data = nullptr;
  EventListener();
  EventListener(RSID ListenerId, EventType Type);
  static EventListener buildAndReg(RSID ListenerId, EventType Type);
  void setListener(RSID lid, std::function<void(RSID, Event)> Func);
  RSID getId() const;
  void setId(RSID ListenerId);
  EventType getType() const;
  void setType(EventType Type);
  virtual void operator()(Event event);
};

#endif //RUNESIM_EVENT_LISTENER_H
