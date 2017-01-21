#include "stdafx.h"
#include "MonitorEvents.h"
#include "Utility.h"
#include <iomanip>

using namespace std;
using namespace VxSdk;
using namespace CppSamples::Common;

Plugin* CppSamples::Events::MonitorEvents::Run(DataModel* dataModel) {

    StartMonitorEvents(dataModel->VxSystem);

    // Return reference of parent plugin to move back to parent menu.
    return GetParent();
}

void CppSamples::Events::MonitorEvents::StartMonitorEvents(IVxSystem* vxSystem) {

    cout << "\n\n" << "Subscribing to events";
    if (!SubscribeEvents(vxSystem)) {
        cout << "Error subscribing to events! \n";
        system("pause");
        return;
    }

    cout << "Monitoring events. Press any key to stop . . .\n\n";
    PrintEventHeadings();
    cin.ignore(256, '\n');
    cin.get();
    cout << "Unsubscribing to events";
    if (!UnSubscribeEvents(vxSystem))
        cout << "Error unsubscribing to events!\n";

    cout << "\n";
    system("pause");
}

/// <summary>
/// Display a row of event details
/// </summary>
/// <param name="vxEvent">CPPConsole::Event pointer containing the event to display</param>
void CppSamples::Events::MonitorEvents::PrintEventRow(IVxEvent* vxEvent) {
    const int eventTimeWidth = 20;
    const int eventStringWidth = 32;
    cout << left << setw(eventTimeWidth) << setfill(' ') << MonitorEvents::CovertUTCTimeFormatToString(vxEvent->time);
    cout << left << setw(eventStringWidth) << setfill(' ') << vxEvent->situationType;
    cout << left << setw(eventStringWidth) << setfill(' ') << vxEvent->sourceDeviceId;
    cout << "\n";
}

/// <summary>
/// Display the headings for a table of events
/// </summary>
void CppSamples::Events::MonitorEvents::PrintEventHeadings() {
    const int eventTimeWidth = 20;
    const int eventStringWidth = 32;
    cout << "\n--------------------------------------------------\n";
    cout << left << setw(eventTimeWidth) << setfill(' ') << "Time(UTC)";
    cout << left << setw(eventStringWidth) << setfill(' ') << "Situation Type";
    cout << left << setw(eventStringWidth) << setfill(' ') << "Source Device";
    cout << "\n--------------------------------------------------\n";
}

/// <summary>
/// Callback method to handle events received from VxSDK
/// </summary>
/// <param name="vxEvent">IVxEvent pointer containing newly received event details</param>
void CppSamples::Events::MonitorEvents::EventsCallBack(IVxEvent* vxEvent) {
    // Only log acknowledgement needed events. Skip others
    if (vxEvent->ackState != VxAckState::kAutoAcked)
        PrintEventRow(vxEvent);
}

/// <summary>
/// Subscribe to event notifications
/// </summary>
/// <param name="vxSystem">system pointer</param>
bool CppSamples::Events::MonitorEvents::SubscribeEvents(IVxSystem* vxSystem) {

    VxResult::Value result = vxSystem->StartNotifications(&MonitorEvents::EventsCallBack);
    if (result == VxResult::kOK) {
        cout << "\n" << "Events subscription successful.\n";
        return true;
    }
    else {
        cout << "\n" << "Failed to subscribe events!!\n";
        return false;
    }
}

/// <summary>
/// Unsubscribe to event notifications
/// </summary>
/// <param name="vxSystem">system pointer</param>
bool CppSamples::Events::MonitorEvents::UnSubscribeEvents(IVxSystem* vxSystem) {

    VxResult::Value result = vxSystem->StopNotifications();
    if (result == VxResult::kOK) {
        cout << "\n" << "Events unsubscription successful.\n";
        return true;
    }
    else {
        cout << "\n" << "Failed to unsubscribe events!!\n";
        return false;
    }
}

/// <summary>
/// Converts the UTC time format to string format (HH:MM::SS, Day Month)
/// </summary>
/// <param name="utcFormat">time string in UTC format (YYYYmmddTHHMMSSZ)</param>
string CppSamples::Events::MonitorEvents::CovertUTCTimeFormatToString(string utcFormat) {
    stringstream dateStream(utcFormat);
    struct tm parseTime;
    //Here parse date and time string value to time structure
    dateStream >> get_time(&parseTime, "%Y-%m-%dT%H:%M:%S");
    char buffer[18];
    strftime(buffer, 18, "%X %x", &parseTime);

    return string(buffer);
}
