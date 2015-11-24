/*
 * Copyright (c) 2015, Robin Burchell <robin@viroteck.net>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#define LOG_TAG "permissioncontroller"

#include <binder/BinderService.h>
#include <binder/IPermissionController.h>
//#include <sensorservice/SensorService.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <powermanager/IPowerManager.h>
#include <schedulingpolicyservice/ISchedulingPolicyService.h>

using namespace android;

namespace android {

class PermissionController : public BinderService<PermissionController>, public BnPermissionController
{
public:
    static const char *getServiceName() { return "permission"; }

    bool checkPermission(const String16& permission, int32_t pid, int32_t uid) {
        ALOGI("Allowing permission '%s' for pid=%d uid=%d\n", String8(permission).string(), pid, uid);
        return true;
    } 
};

class PowerManager : public BinderService<PowerManager>, public BnPowerManager
{
public:
    static const char *getServiceName() { return "power"; }
    // Used in 4.4 
    virtual status_t acquireWakeLock(const sp<IBinder>& lock, int flags, const String16& tag, const String16& packageName, int ws) { return OK; }
    // Used in 5.0  
    virtual status_t acquireWakeLock(const sp<IBinder>& lock, int flags, const String16& tag, const String16& packageName, int ws, const String16& historyTag) { return OK; }
    virtual status_t acquireWakeLockWithUid(const sp<IBinder>& lock, int flags, const String16& tag, const String16& packageName, int uidtoblame) { return OK; };
    virtual status_t releaseWakeLock(const sp<IBinder>& lock, int flags) { return OK; }
    virtual status_t updateWakeLockUids(const sp<IBinder>& lock, int uids) { return OK; }
    virtual status_t powerHint(int hintId, int data) { return OK; } 

    // Used in 4.4 
    virtual status_t updateWakeLockint(const sp<IBinder>& lock, int ws) { return OK; } 
    // Used in 5.0 
    virtual status_t updateWakeLockint(const sp<IBinder>& lock, int ws, const String16& historyTag) { return OK; }

    virtual bool isWakeLockLevelSupported(int level) { return true; }
    virtual status_t userActivity(long time, int event, int flags) { return OK; }
    virtual status_t wakeUp(long time) { return OK; }
    
    // Used in 4.4
    virtual status_t goToSleep(long time, int reason) { return OK; }
    // Used in 5.0
    virtual status_t goToSleep(long time, int reason, int flags) { return OK; }
    virtual status_t nap(long time) { return OK; }
    
    // Used in 4.4
    virtual bool isScreenOn() { return true; }
     
    virtual bool isInteractive() { return false; }
    virtual bool isPowerSaveMode() { return false; }
    virtual bool setPowerSaveMode(bool mode) { return true; }
    virtual status_t reboot(bool confirm, const String16& reason, bool wait) { return OK; }
    virtual status_t shutdown(bool confirm, bool wait) { return OK; }
    virtual status_t crash(const String16& message) { return OK; }
    virtual status_t setStayOnSetting(int val) { return OK; }
    virtual status_t setMaximumScreenOffTimeoutFromDeviceAdmin(int timeMs) { return OK; }
    virtual status_t setTemporaryScreenBrightnessSettingOverride(int brightness) { return OK; }
    virtual status_t setTemporaryScreenAutoBrightnessAdjustmentSettingOverride(float adj) { return OK; }
    virtual status_t setAttentionLight(bool on, int color) { return OK; }

    // Used in 5.1
    virtual status_t boostScreenBrightness(long time) { return OK; }
};

class SchedulingPolicyService : public BinderService<SchedulingPolicyService>, public BnSchedulingPolicyService
{
public:
    static const char *getServiceName() { return "scheduling_policy"; }

    virtual int requestPriority(int pid, int tid, int prio) { return 0; }
};


};

int main(int argc, char **argv)
{
    ALOGI("Starting up");
    sp<ProcessState> proc(ProcessState::self());
    sp<IServiceManager> sm = defaultServiceManager();
    ALOGI("ServiceManager: %p", sm.get());

    android::PermissionController::instantiate();
//    android::SensorService::instantiate()
    android::PowerManager::instantiate();
    android::SchedulingPolicyService::instantiate();
    ProcessState::self()->startThreadPool();
    IPCThreadState::self()->joinThreadPool();
   
    return 0;
}

