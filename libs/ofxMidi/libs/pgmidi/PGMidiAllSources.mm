//
//  PGMidiAllSources.mm
//  PGMidi
//

#import "PGMidiAllSources.h"

#import "PGMidi.h"
#import "PGArc.h"

@interface PGMidiAllSources () <PGMidiDelegate, PGMidiSourceDelegate>
@end

@implementation PGMidiAllSources

- (void) dealloc
{
    self.midi = nil;
#if ! PGMIDI_ARC
    [super dealloc];
#endif
}

@synthesize midi, delegate;

- (void) setMidi:(PGMidi *)newMidi
{
    midi.delegate = nil;
    for (PGMidiSource *source in midi.sources) [source removeDelegate:self];

    midi = newMidi;

    midi.delegate = self;
    for (PGMidiSource *source in midi.sources) [source addDelegate:self];
}

#pragma mark PGMidiDelegate

- (void) midi:(PGMidi*)midi sourceAdded:(PGMidiSource *)source
{
    [source addDelegate:self];
}

- (void) midi:(PGMidi*)midi sourceRemoved:(PGMidiSource *)source {}
- (void) midi:(PGMidi*)midi destinationAdded:(PGMidiDestination *)destination {}
- (void) midi:(PGMidi*)midi destinationRemoved:(PGMidiDestination *)destination {}

#pragma mark PGMidiSourceDelegate

- (void) midiSource:(PGMidiSource*)input midiReceived:(const MIDIPacketList *)packetList
{
    [delegate midiSource:input midiReceived:packetList];
}

@end
