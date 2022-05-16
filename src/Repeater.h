#pragma once

#include "mesh/Channels.h"
#include "mesh/MeshTypes.h"
#include "mesh/RadioInterface.h"

/**
 * A mesh aware repeatere that supports multiple interfaces.
 */
class Repeater 
{
  protected:
    RadioInterface *radioInterface = NULL;

  public:
    /**
     * Constructor
     *
     */
    Repeater();

    void init();

  protected:
    /**
     * Currently we only allow one interface, that may change in the future
     */
    void addInterface(RadioInterface *_iface) { radioInterface = _iface; }

    virtual ErrorCode send(MeshPacket *p);

        /** Attempt to cancel a previously sent packet.  Returns true if a packet was found we could cancel */
    bool cancelSending(NodeNum from, PacketId id);
};

extern Repeater *repeater;

