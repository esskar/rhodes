//
//     Generated by class-dump 3.5 (64 bit).
//
//     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2013 by Steve Nygard.
//

#pragma mark Blocks

typedef void (^CDUnknownBlockType)(void); // return type and parameters are unknown

#pragma mark -

//
// File: /Applications/Xcode.app/Contents/SharedFrameworks/DVTiPhoneSimulatorRemoteClient.framework/Versions/A/DVTiPhoneSimulatorRemoteClient
// UUID: 5AB00B0E-FF6C-3754-A826-0C89B7EF2345
//
//                           Arch: x86_64
//                Current version: 12.0.0
//          Compatibility version: 1.0.0
//                 Source version: 6175.19.0.0.0
//       Minimum Mac OS X version: 10.9.0
//                    SDK version: 10.10.0
//
// Objective-C Garbage Collection: Unsupported
//
//                       Run path: @loader_path/../../../
//                               = /Applications/Xcode.app/Contents/SharedFrameworks
//                       Run path: @loader_path/../../../../PrivateFrameworks/
//                               = /Applications/Xcode.app/Contents/PrivateFrameworks
//                       Run path: @loader_path/../../../../Developer/Library/PrivateFrameworks/
//                               = /Applications/Xcode.app/Contents/Developer/Library/PrivateFrameworks
//

@protocol DVTInvalidation <NSObject>
- (void)primitiveInvalidate;

@optional
@property(retain) DVTStackBacktrace *creationBacktrace;
@property(readonly) DVTStackBacktrace *invalidationBacktrace;
@property(readonly, nonatomic, getter=isValid) BOOL valid;
- (void)invalidate;
@end

@protocol NSCopying
- (id)copyWithZone:(struct _NSZone *)arg1;
@end

@protocol NSObject
@property(readonly, copy) NSString *description;
@property(readonly) Class superclass;
@property(readonly) unsigned long long hash;
- (struct _NSZone *)zone;
- (unsigned long long)retainCount;
- (id)autorelease;
- (oneway void)release;
- (id)retain;
- (BOOL)respondsToSelector:(SEL)arg1;
- (BOOL)conformsToProtocol:(Protocol *)arg1;
- (BOOL)isMemberOfClass:(Class)arg1;
- (BOOL)isKindOfClass:(Class)arg1;
- (BOOL)isProxy;
- (id)performSelector:(SEL)arg1 withObject:(id)arg2 withObject:(id)arg3;
- (id)performSelector:(SEL)arg1 withObject:(id)arg2;
- (id)performSelector:(SEL)arg1;
- (id)self;
- (Class)class;
- (BOOL)isEqual:(id)arg1;

@optional
@property(readonly, copy) NSString *debugDescription;
@end

@interface DVTiPhoneSimulatorMessenger : NSObject <DVTInvalidation>
{
    DTiPhoneSimulatorSession *_session;
    CDUnknownBlockType _readyMessageHandler;
    CDUnknownBlockType _runningMessageHandler;
    CDUnknownBlockType _appDidLaunchMessageHandler;
    CDUnknownBlockType _toolDidLaunchMessageHandler;
    CDUnknownBlockType _appDidQuitMessageHandler;
}

+ (id)messengerForSession:(id)arg1 withConnection:(id)arg2;
+ (id)messengerForSession:(id)arg1;
+ (void)initialize;
@property(copy, nonatomic) CDUnknownBlockType appDidQuitMessageHandler; // @synthesize appDidQuitMessageHandler=_appDidQuitMessageHandler;
@property(copy, nonatomic) CDUnknownBlockType toolDidLaunchMessageHandler; // @synthesize toolDidLaunchMessageHandler=_toolDidLaunchMessageHandler;
@property(copy, nonatomic) CDUnknownBlockType appDidLaunchMessageHandler; // @synthesize appDidLaunchMessageHandler=_appDidLaunchMessageHandler;
@property(copy, nonatomic) CDUnknownBlockType runningMessageHandler; // @synthesize runningMessageHandler=_runningMessageHandler;
@property(copy, nonatomic) CDUnknownBlockType readyMessageHandler; // @synthesize readyMessageHandler=_readyMessageHandler;
@property(readonly) __weak DTiPhoneSimulatorSession *session; // @synthesize session=_session;

- (void)doUbiquityFetchEvent;
- (void)doFetchEventForPID:(int)arg1;
- (void)backgroundAllApps:(int)arg1;
- (void)startSimulatorToolSessionWithParameters:(id)arg1;
- (void)stopSimulatingLocation;
- (void)startSimulatingLocationWithLatitude:(id)arg1 longitute:(id)arg2;
- (void)endSimulatorSessionWithPID:(int)arg1;
- (void)startSimulatorSessionWithRequestInfo:(id)arg1;
- (id)spawnToolWithConfig:(id)arg1;
- (id)launchApplicationWithConfig:(id)arg1;
- (id)installApplicationWithConfig:(id)arg1;
- (id)startSessionWithConfig:(id)arg1;
- (void)clearAllMessageHandlers;
- (void)waitPID:(int)arg1 withAppPIDExitedMessagedHandler:(CDUnknownBlockType)arg2;
- (void)disconnectFromService;
- (BOOL)connectToServiceWithSessionOnLaunch:(BOOL)arg1 simulatorPID:(int *)arg2 error:(id *)arg3;
- (id)initWithSession:(id)arg1;
- (void)primitiveInvalidate;

// Remaining properties
@property(retain) DVTStackBacktrace *creationBacktrace;
@property(readonly, copy) NSString *debugDescription;
@property(readonly, copy) NSString *description;
@property(readonly) unsigned long long hash;
@property(readonly) DVTStackBacktrace *invalidationBacktrace;
@property(readonly) Class superclass;
@property(readonly, nonatomic, getter=isValid) BOOL valid;

@end



@interface DVTiPhoneSimulatorLocalMessenger : DVTiPhoneSimulatorMessenger
{
    _Bool _appTerminationMessageSent;
    NSObject<OS_dispatch_source> *_pidDispatchSource;
}

+ (id)allocWithZone:(struct _NSZone *)arg1;

- (void)doUbiquityFetchEvent;
- (void)doFetchEventForPID:(int)arg1;
- (void)waitPID:(int)arg1 withAppPIDExitedMessagedHandler:(CDUnknownBlockType)arg2;
- (void)didReceiveSimulatorRunningNote:(id)arg1;
- (void)didReceiveSimulatorReadyNote:(id)arg1;
- (void)stopSimulatingLocation;
- (void)startSimulatingLocationWithLatitude:(id)arg1 longitute:(id)arg2;
- (void)endSimulatorSessionWithPID:(int)arg1;
- (void)clearAllMessageHandlers;
- (void)disconnectFromService;
- (BOOL)connectToServiceWithSessionOnLaunch:(BOOL)arg1 simulatorPID:(int *)arg2 error:(id *)arg3;
- (void)primitiveInvalidate;
- (id)initWithSession:(id)arg1;

@end


@interface DVTiPhoneSimulatorDistNoteMessenger : DVTiPhoneSimulatorLocalMessenger
{
    DVTNotificationToken *_appDidLaunchMessageToken;
    DVTNotificationToken *_toolDidLaunchMessageToken;
}

+ (void)initialize;

- (void)backgroundAllApps:(int)arg1;
- (void)startSimulatorToolSessionWithParameters:(id)arg1;
- (void)startSimulatorSessionWithRequestInfo:(id)arg1;
- (void)setToolDidLaunchMessageHandler:(CDUnknownBlockType)arg1;
- (void)setAppDidLaunchMessageHandler:(CDUnknownBlockType)arg1;
- (void)primitiveInvalidate;

@end



@interface DVTiPhoneSimulatorRemoteMessenger : DVTiPhoneSimulatorMessenger
{
    unsigned long long _commandTag;
    NSObject<OS_dispatch_queue> *_responseQueue;
    DVTDispatchLock *_awaitingLock;
    NSMutableDictionary *_awaitingSemaphores;
    NSMutableDictionary *_awaitingResponses;
    NSMutableSet *_waitingAppPIDs;
    CDUnknownBlockType _appPIDExitedMessageHandler;
    DVTConfinementServiceConnection *_connection;
}

@property(readonly) DVTConfinementServiceConnection *connection; // @synthesize connection=_connection;

- (void)handleNotificationResponse:(id)arg1;
- (void)waitPID:(int)arg1 withAppPIDExitedMessagedHandler:(CDUnknownBlockType)arg2;
- (void)startSimulatorToolSessionWithParameters:(id)arg1;
- (void)stopSimulatingLocation;
- (void)startSimulatingLocationWithLatitude:(id)arg1 longitute:(id)arg2;
- (void)endSimulatorSessionWithPID:(int)arg1;
- (void)startSimulatorSessionWithRequestInfo:(id)arg1;
- (void)disconnectFromService;
- (BOOL)connectToServiceWithSessionOnLaunch:(BOOL)arg1 simulatorPID:(int *)arg2 error:(id *)arg3;
- (BOOL)sendTaggedRequest:(id)arg1 awaitingResponse:(id *)arg2 error:(id *)arg3;
- (id)nextCommandTag;
- (id)awaitResponseWithTag:(id)arg1 error:(id *)arg2;
- (void)enqueueResponse:(id)arg1 withTag:(id)arg2 error:(id)arg3;
- (BOOL)sendRequest:(id)arg1 withTag:(id)arg2 error:(id *)arg3;
- (id)initWithSession:(id)arg1 connection:(id)arg2;

@end

@interface DTiPhoneSimulatorSession : NSObject <DVTInvalidation>
{
    int _simulatedApplicationPID;
    int _simulatorPID;
    id <DTiPhoneSimulatorSessionDelegate> _delegate;
    NSString *_simulatedAppPath;
    NSString *_uuid;
    DTiPhoneSimulatorSessionConfig *_sessionConfig;
    long long _sessionLifecycleProgress;
    NSTimer *_timeoutTimer;
    DVTiPhoneSimulatorMessenger *_messenger;
}

+ (void)initialize;
@property(retain) DVTiPhoneSimulatorMessenger *messenger; // @synthesize messenger=_messenger;
@property(retain, nonatomic) NSTimer *timeoutTimer; // @synthesize timeoutTimer=_timeoutTimer;
@property(nonatomic) long long sessionLifecycleProgress; // @synthesize sessionLifecycleProgress=_sessionLifecycleProgress;
@property(copy, nonatomic) DTiPhoneSimulatorSessionConfig *sessionConfig; // @synthesize sessionConfig=_sessionConfig;
@property(readonly, copy, nonatomic) NSString *uuid; // @synthesize uuid=_uuid;
@property int simulatorPID; // @synthesize simulatorPID=_simulatorPID;
@property(copy) NSString *simulatedAppPath; // @synthesize simulatedAppPath=_simulatedAppPath;
@property int simulatedApplicationPID; // @synthesize simulatedApplicationPID=_simulatedApplicationPID;
@property(retain, nonatomic) id <DTiPhoneSimulatorSessionDelegate> delegate; // @synthesize delegate=_delegate;

- (void)doUbiquityFetchEvent;
- (void)doFetchEventForPID:(int)arg1;
- (void)backgroundAllApps:(int)arg1;
- (id)_invalidConfigError;
- (void)_endSimulatorSession;
- (void)_callDelegateResponseFromSessionStartedWithPID:(int)arg1 andError:(id)arg2;
- (id)_sessionStartRequestInfoFromConfig:(id)arg1 withError:(id *)arg2;
- (BOOL)_startToolSessionInSimulatorWithError:(id *)arg1;
- (BOOL)_startApplicationSessionInSimulatorWithError:(id *)arg1;
- (BOOL)_startBasicSessionInSimulatorWithError:(id *)arg1;
- (BOOL)_startSessionInSimulatorWithError:(id *)arg1;
- (void)_handleSessionEndedWithError:(id)arg1;
- (void)_timeoutElapsed:(id)arg1;
- (BOOL)attachedToTargetWithConfig:(id)arg1 error:(id *)arg2;
- (void)stopLocationSimulation;
- (void)simulateLocationWithLatitude:(id)arg1 longitude:(id)arg2;
- (void)requestEndWithTimeout:(double)arg1;
- (BOOL)requestStartWithConfig:(id)arg1 timeout:(double)arg2 error:(id *)arg3;
- (BOOL)_setUpSimulatorMessengerWithConfig:(id)arg1 error:(id *)arg2;
@property(readonly, copy) NSString *description;
- (id)init;
- (void)primitiveInvalidate;

// Remaining properties
@property(retain) DVTStackBacktrace *creationBacktrace;
@property(readonly, copy) NSString *debugDescription;
@property(readonly) unsigned long long hash;
@property(readonly) DVTStackBacktrace *invalidationBacktrace;
@property(readonly) Class superclass;
@property(readonly, nonatomic, getter=isValid) BOOL valid;

@end

@interface DTiPhoneSimulatorSessionConfig : NSObject <NSCopying>
{
    BOOL _simulatedApplicationShouldWaitForDebugger;
    BOOL _launchForBackgroundFetch;
    NSString *_localizedClientName;
    SimRuntime *_runtime;
    SimDevice *_device;
    NSString *_simulatedDeviceInfoName;
    NSNumber *_simulatedDeviceFamily;
    NSString *_simulatedArchitecture;
    NSNumber *_simulatedDisplayHeight;
    NSNumber *_simulatedDisplayScale;
    DTiPhoneSimulatorApplicationSpecifier *_applicationToSimulateOnStart;
    NSNumber *_pid;
    NSArray *_simulatedApplicationLaunchArgs;
    NSDictionary *_simulatedApplicationLaunchEnvironment;
    NSString *_simulatedApplicationStdOutPath;
    NSString *_simulatedApplicationStdErrPath;
    NSFileHandle *_stdinFileHandle;
    NSFileHandle *_stdoutFileHandle;
    NSFileHandle *_stderrFileHandle;
    id _confinementService;
}

+ (id)displayNameForDeviceFamily:(id)arg1;
@property BOOL launchForBackgroundFetch; // @synthesize launchForBackgroundFetch=_launchForBackgroundFetch;
@property(retain) id confinementService; // @synthesize confinementService=_confinementService;
@property(retain) NSFileHandle *stderrFileHandle; // @synthesize stderrFileHandle=_stderrFileHandle;
@property(retain) NSFileHandle *stdoutFileHandle; // @synthesize stdoutFileHandle=_stdoutFileHandle;
@property(retain) NSFileHandle *stdinFileHandle; // @synthesize stdinFileHandle=_stdinFileHandle;
@property(copy) NSString *simulatedApplicationStdErrPath; // @synthesize simulatedApplicationStdErrPath=_simulatedApplicationStdErrPath;
@property(copy) NSString *simulatedApplicationStdOutPath; // @synthesize simulatedApplicationStdOutPath=_simulatedApplicationStdOutPath;
@property BOOL simulatedApplicationShouldWaitForDebugger; // @synthesize simulatedApplicationShouldWaitForDebugger=_simulatedApplicationShouldWaitForDebugger;
@property(copy) NSDictionary *simulatedApplicationLaunchEnvironment; // @synthesize simulatedApplicationLaunchEnvironment=_simulatedApplicationLaunchEnvironment;
@property(copy) NSArray *simulatedApplicationLaunchArgs; // @synthesize simulatedApplicationLaunchArgs=_simulatedApplicationLaunchArgs;
@property(copy) NSNumber *pid; // @synthesize pid=_pid;
@property(copy) DTiPhoneSimulatorApplicationSpecifier *applicationToSimulateOnStart; // @synthesize applicationToSimulateOnStart=_applicationToSimulateOnStart;
@property(copy) NSNumber *simulatedDisplayScale; // @synthesize simulatedDisplayScale=_simulatedDisplayScale;
@property(copy) NSNumber *simulatedDisplayHeight; // @synthesize simulatedDisplayHeight=_simulatedDisplayHeight;
@property(copy) NSString *simulatedArchitecture; // @synthesize simulatedArchitecture=_simulatedArchitecture;
@property(copy) NSNumber *simulatedDeviceFamily; // @synthesize simulatedDeviceFamily=_simulatedDeviceFamily;
@property(retain) NSString *simulatedDeviceInfoName; // @synthesize simulatedDeviceInfoName=_simulatedDeviceInfoName;
@property(retain) SimDevice *device; // @synthesize device=_device;
@property(retain) SimRuntime *runtime; // @synthesize runtime=_runtime;
@property(copy) NSString *localizedClientName; // @synthesize localizedClientName=_localizedClientName;

- (id)description;
@property(copy) DTiPhoneSimulatorSystemRoot *simulatedSystemRoot;
- (id)copyWithZone:(struct _NSZone *)arg1;
- (id)init;

@end

@interface DTiPhoneSimulatorSystemRoot : NSObject <NSCopying>
{
    SimRuntime *_runtime;
}

+ (id)rootWithSDKVersion:(id)arg1;
+ (id)rootWithSDKPath:(id)arg1;
+ (id)rootWithSimRuntime:(id)arg1;
+ (id)defaultRoot;
+ (id)knownRoots;
+ (void)initialize;
@property(readonly) SimRuntime *runtime; // @synthesize runtime=_runtime;

- (id)description;
- (long long)compare:(id)arg1;
- (id)copyWithZone:(struct _NSZone *)arg1;
- (BOOL)isEqual:(id)arg1;
@property(readonly, copy) NSString *sdkVersion;
@property(readonly, copy) NSString *sdkDisplayName;
@property(readonly, copy) NSString *sdkRootPath;
- (id)initWithRuntime:(id)arg1;

@end

@interface DTiPhoneSimulatorApplicationSpecifier : NSObject <NSCopying>
{
}

+ (id)specifierWithToolPath:(id)arg1;
+ (id)specifierWithApplicationBundleIdentifier:(id)arg1;
+ (id)specifierWithApplicationPath:(id)arg1;
- (BOOL)isTool;
- (id)description;
@property(readonly, copy) NSString *toolPath;
@property(readonly, copy) NSString *bundleID;
@property(readonly, copy) NSString *appPath;
- (id)copyWithZone:(struct _NSZone *)arg1;

@end

@interface DTiPhoneSimulatorApplicationSpecifier_Path : DTiPhoneSimulatorApplicationSpecifier
{
    NSString *_appPath;
}

+ (id)specifierWithApplicationPath:(id)arg1;
- (id)appPath;

- (id)description;

@end

@interface DTiPhoneSimulatorApplicationSpecifier_BundleIdentifier : DTiPhoneSimulatorApplicationSpecifier
{
    NSString *_bundleID;
}

+ (id)specifierWithApplicationBundleIdentifier:(id)arg1;
- (id)bundleID;

- (id)description;

@end

@interface DTiPhoneSimulatorApplicationSpecifier_ToolPath : DTiPhoneSimulatorApplicationSpecifier
{
    NSString *_toolPath;
}

+ (id)specifierWithToolPath:(id)arg1;
- (id)toolPath;

- (id)description;
- (BOOL)isTool;

@end

@interface DVTiPhoneSimulatorCoreSimMessenger : DVTiPhoneSimulatorLocalMessenger
{
}

- (void)doUbiquityFetchEvent;
- (void)doFetchEventForPID:(int)arg1;
- (void)backgroundAllApps:(int)arg1;
- (void)stopSimulatingLocation;
- (void)startSimulatingLocationWithLatitude:(id)arg1 longitute:(id)arg2;
- (id)spawnToolWithConfig:(id)arg1;
- (id)launchApplicationWithConfig:(id)arg1;
- (id)installApplicationWithConfig:(id)arg1;
- (id)startSessionWithConfig:(id)arg1;
- (void)startSimulatorToolSessionWithParameters:(id)arg1;
- (void)startSimulatorSessionWithRequestInfo:(id)arg1;

@end

@interface SimRuntime (DVTAdditions)
+ (id)dvt_runtimeFromSDKRoot:(id)arg1;
@end

@interface SimDeviceType (DVTAdditions)
- (id)dvt_supportedArchs;
- (id)dvt_supportedArchStrings;
- (_Bool)dvt_has64BitArch;
@end

@interface SimDeviceSet (DVTAdditions)
- (id)dvt_registerNotificationHandlerOnQueue:(id)arg1 handler:(CDUnknownBlockType)arg2;
- (id)dvt_registerNotificationHandler:(CDUnknownBlockType)arg1;
@end

@interface SimDevice (DVTAdditions)
- (id)dvt_launchApplicationWithBundleIdentifier:(id)arg1 andOptions:(id)arg2;
- (id)dvt_launchApplicationWithBundleIdentifier:(id)arg1;
- (id)dvt_spawnExecutableAtPath:(id)arg1 withOptions:(id)arg2 andTerminationHandler:(CDUnknownBlockType)arg3;
- (id)dvt_spawnExecutableAtPath:(id)arg1 withOptions:(id)arg2;
- (id)dvt_uninstallApplicationWithBundleIdentifier:(id)arg1 andOptions:(id)arg2;
- (id)dvt_uninstallApplicationWithBundleIdentifier:(id)arg1;
- (id)dvt_installApplicationAtPath:(id)arg1 withOptions:(id)arg2;
- (id)dvt_installApplicationAtPath:(id)arg1;
- (id)dvt_restoreContentsAndSettingsFrom:(id)arg1;
- (id)dvt_eraseContentsAndSettings;
- (id)dvt_shutdown;
- (id)dvt_bootWithOptions:(id)arg1;
- (id)dvt_boot;
- (id)dvt_notifyOfBootOnQueue:(id)arg1 handler:(CDUnknownBlockType)arg2;
- (id)dvt_registerNotificationHandlerOnQueue:(id)arg1 handler:(CDUnknownBlockType)arg2;
- (id)dvt_registerNotificationHandler:(CDUnknownBlockType)arg1;
@end
