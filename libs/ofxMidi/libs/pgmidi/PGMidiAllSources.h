//
//  PGMidiAllSources.h
//  PGMidi
//

#import <Foundation/Foundation.h>

@class PGMidi;
@protocol PGMidiSourceDelegate;

@interface PGMidiAllSources : NSObject
{
    PGMidi                  *midi;
    id<PGMidiSourceDelegate> delegate;
}

#if ! __has_feature(objc_arc)

@property (nonatomic,assign) PGMidi *midi;
@property (nonatomic,assign) id<PGMidiSourceDelegate> delegate;

#else

@property (nonatomic,strong) PGMidi *midi;
@property (nonatomic,strong) id<PGMidiSourceDelegate> delegate;

#endif

@end
