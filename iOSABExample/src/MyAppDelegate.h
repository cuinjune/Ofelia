#import "ofxiOSAppDelegate.h"
#import "ofxiOSViewController.h"
#import <Foundation/Foundation.h>
#import "Audiobus.h"

@interface MyAppDelegate : ofxiOSAppDelegate

@property (nonatomic, strong) ofxiOSViewController * rootViewController;

@property (strong, nonatomic) ABAudiobusController *audiobusController;

@property (strong, nonatomic) ABAudioSenderPort * audiobusSender;

@end

