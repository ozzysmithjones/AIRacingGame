#pragma once
#include "DrawableGameObject.h"
class Waypoint :
    public DrawableGameObject
{
public:
    virtual HRESULT initMesh(ID3D11Device* pd3dDevice, const int index);

    int getCheckpointIndex() const { return m_checkpointIndex; }
    bool isOnTrack() const { return m_isOnTrack; }
    bool isCheckPoint() const { return m_isCheckpoint; }
    void setVisible(const bool visible) { m_visible = visible; }
    bool isVisible() {return m_visible;}

private:
    
    int  m_checkpointIndex;
    bool m_isOnTrack;
    bool m_isCheckpoint;
    bool m_visible;
};

