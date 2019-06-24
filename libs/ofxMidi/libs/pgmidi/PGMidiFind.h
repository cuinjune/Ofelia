//
//  PGMidiFind.h
//  PGMidi
//

#import "PGMidi.h"

@interface PGMidi (FindingConnections)

- (PGMidiSource*)      findSourceCalled:(NSString*)name;

- (PGMidiDestination*) findDestinationCalled:(NSString*)name;

- (void)               findMatchingSource:(PGMidiSource**)source
                           andDestination:(PGMidiDestination**)destination;

- (void)               findMatchingSource:(PGMidiSource**)source
                           andDestination:(PGMidiDestination**)destination
                               avoidNames:(NSArray*)namesToAvoid;

@end
