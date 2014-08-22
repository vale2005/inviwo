/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Main file authors: Rickard Englund, Hans-Christian Helltegen
 *
 *********************************************************************************/

#ifndef IVW_TRACKBALLLIGHT_H
#define IVW_TRACKBALLLIGHT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/interaction/interactionhandler.h>
#include <inviwo/core/interaction/events/event.h>
#include <inviwo/core/interaction/events/gestureevent.h>
#include <inviwo/core/interaction/events/mouseevent.h>
#include <inviwo/core/interaction/events/keyboardevent.h>
#include <inviwo/core/interaction/events/eventlistener.h>
#include <inviwo/core/interaction/events/resizeevent.h>
#include <inviwo/core/io/serialization/ivwserializable.h>
#include <inviwo/core/properties/propertyowner.h>
#include <inviwo/core/properties/eventproperty.h>
#include <inviwo/core/interaction/events/event.h>
#include <inviwo/core/interaction/trackballaction.h>
#include <inviwo/core/util/observer.h>

namespace inviwo {
class TrackballLight;
/** \class TrackballObserver
 *
 * Trackball observer that gets notified when positions and directions change.
 *
 * @see TrackballObservable
 */
class IVW_CORE_API TrackballObserver: public Observer {
public:
    TrackballObserver(): Observer() {};
    virtual void onAllTrackballChanged(const TrackballLight* trackball) {};
    virtual void onLookFromChanged(const TrackballLight* trackball) {};
    virtual void onLookToChanged(const TrackballLight* trackball) {};
    virtual void onLookUpChanged(const TrackballLight* trackball) {};
};


/** \class TrackballObservable
 *
 * Can call notifyObserversInvalidationBegin and notifyObserversInvalidationEnd
 *
 * @see TrackballObserver
 */
class IVW_CORE_API TrackballObservable: public Observable<TrackballObserver> {
public:
    TrackballObservable(): Observable<TrackballObserver>() {};

    void notifyAllChanged(const TrackballLight* trackball) const {
        // Notify observers
        for (ObserverSet::iterator it = observers_->begin(), itEnd = observers_->end(); it != itEnd; ++it) {
            // static_cast can be used since only template class objects can be added
            static_cast<TrackballObserver*>(*it)->onAllTrackballChanged(trackball);
        }
    }
    void notifyLookFromChanged(const TrackballLight* trackball) const {
        // Notify observers
        for (ObserverSet::iterator it = observers_->begin(), itEnd = observers_->end(); it != itEnd; ++it) {
            // static_cast can be used since only template class objects can be added
            static_cast<TrackballObserver*>(*it)->onLookFromChanged(trackball);
        }
    }
    void notifyLookToChanged(const TrackballLight* trackball) const {
        // Notify observers
        for (ObserverSet::iterator it = observers_->begin(), itEnd = observers_->end(); it != itEnd; ++it) {
            // static_cast can be used since only template class objects can be added
            static_cast<TrackballObserver*>(*it)->onLookToChanged(trackball);
        }
    }
    void notifyLookUpChanged(const TrackballLight* trackball) const {
        // Notify observers
        for (ObserverSet::iterator it = observers_->begin(), itEnd = observers_->end(); it != itEnd; ++it) {
            // static_cast can be used since only template class objects can be added
            static_cast<TrackballObserver*>(*it)->onLookUpChanged(trackball);
        }
    }
};

class IVW_CORE_API TrackballLight : public InteractionHandler, public PropertyOwner, public TrackballObservable {

public:
    TrackballLight(vec3* lookFrom, vec3* lookTo, vec3* lookUp);
    ~TrackballLight();

    virtual void invokeEvent(Event* event);
    void addProperty(Property& property);
    void addProperty(Property* property);

    virtual std::string getClassName() const { return "TrackballLight"; }

    void serialize(IvwSerializer& s) const;
    void deserialize(IvwDeserializer& d);

    vec3* getLookTo() { return lookTo_; }
    vec3* getLookFrom() { return lookFrom_; }
    vec3* getLookUp() { return lookUp_; }
    const vec3* getLookTo() const { return lookTo_; }
    const vec3* getLookFrom() const { return lookFrom_; }
    const vec3* getLookUp() const { return lookUp_; }

private:
    enum Direction {
        UP = 0,
        LEFT,
        DOWN,
        RIGHT
    };

    float pixelWidth_;
    bool isMouseBeingPressedAndHold_;

    vec2 lastMousePos_;
    vec3 lastTrackballLightPos_;

    vec3* lookFrom_;
    vec3* lookTo_;
    vec3* lookUp_;




    MouseEvent rotateEvent_;
    MouseEvent zoomEvent_;
    MouseEvent panEvent_;

    KeyboardEvent stepRotateUpEvent_;
    KeyboardEvent stepRotateLeftEvent_;
    KeyboardEvent stepRotateDownEvent_;
    KeyboardEvent stepRotateRightEvent_;

    KeyboardEvent stepZoomInEvent_;
    KeyboardEvent stepZoomOutEvent_;
    KeyboardEvent stepPanUpEvent_;
    KeyboardEvent stepPanLeftEvent_;
    KeyboardEvent stepPanDownEvent_;
    KeyboardEvent stepPanRightEvent_;



    TrackballAction rotateAction_;
    TrackballAction zoomAction_;
    TrackballAction panAction_;

    TrackballAction stepRotateUpAction_;
    TrackballAction stepRotateLeftAction_;
    TrackballAction stepRotateDownAction_;
    TrackballAction stepRotateRightAction_;

    TrackballAction stepZoomInAction_;
    TrackballAction stepZoomOutAction_;
    TrackballAction stepPanUpAction_;
    TrackballAction stepPanLeftAction_;
    TrackballAction stepPanDownAction_;
    TrackballAction stepPanRightAction_;


    EventProperty rotateEventProperty_;
    EventProperty zoomEventProperty_;
    EventProperty panEventProperty_;

    EventProperty stepRotateUpProperty_;
    EventProperty stepRotateLeftProperty_;
    EventProperty stepRotateDownProperty_;
    EventProperty stepRotateRightProperty_;

    EventProperty stepZoomInProperty_;
    EventProperty stepZoomOutProperty_;
    EventProperty stepPanUpProperty_;
    EventProperty stepPanLeftProperty_;
    EventProperty stepPanDownProperty_;
    EventProperty stepPanRightProperty_;

    vec3 mapNormalizedMousePosToTrackballLight(vec2 mousePos, float dist = 1.f);
    vec3 mapToCamera(vec3 pos, float dist = 1.f);
    void rotateCamera(MouseEvent* mouseEvent);

    void rotateFromPosToPos(vec3 currentCamPos, vec3 nextCamPos, float rotationAngle);

    void zoomCamera(MouseEvent* mouseEvent);
    void panCamera(MouseEvent* mouseEvent);
    void stepRotateCamera(Direction dir);
    void stepZoomCamera(Direction dir);
    void stepPanCamera(Direction dir);
};

} // namespace

#endif // IVW_TRACKBALLLIGHT_H