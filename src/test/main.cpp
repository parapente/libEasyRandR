#include <QTextStream>
#include "configuration.h"

int main(int argc, char **argv) {
    QTextStream out(stdout);
    out << "EasyRandR: \"Hello, world!\"\n";
    EasyRandR::Configuration cfg;
    
    QMap<RROutput,EasyRandR::Output*> outlist;
    outlist = cfg.getOutputs(0);
    out << outlist.count() << " outputs found!\n";
    QMapIterator<RROutput,EasyRandR::Output*> outlist_it(outlist);
    while(outlist_it.hasNext()) {
	outlist_it.next();
	
	// Output Name
	out << "\n" << outlist_it.key() << " --> " << outlist_it.value()->name() << "\n";
	
	// Output Modes
	out << "\nModes\n-----\n";
	QMap<RRMode,QString> l;
	l = outlist_it.value()->validModes();
	QMapIterator<RRMode,QString> it(l);
	while(it.hasNext()) {
	    it.next();
	    out << it.key() << ":" << it.value() << "\n";
	}
	out << "The " << outlist_it.value()->preferedModes().count() << " first mode(s) are prefered\n";
	out << "Current mode: " << l.value(outlist_it.value()->currentMode()) << "\n";
	out << "Width:" << outlist_it.value()->width() << ", Height:" << outlist_it.value()->height()
	    << ", X:" << outlist_it.value()->x() << ", Y:" << outlist_it.value()->y() << "\n";
	
	// Output Clones
	QList<RROutput> clones;
	clones = outlist_it.value()->clones();
	out << "Clones:";
	for (int i=0; i<clones.count(); i++)
	    out << " " << outlist.value(clones[i])->name();
	out << "\n";
	
	// Output Connection & Crtc
	if (outlist_it.value()->connectionStatus() == RR_Connected)
	    out << "Connected to CRTC: " << outlist_it.value()->crtc() << "\n";
	else if (outlist_it.value()->connectionStatus() == RR_Disconnected)
	    out << "Disconnected\n";
	else
	    out << "Unknown connection status!\n";
	
	// Output Valid Crtcs
	QList<RRCrtc> vcrtcs;
	vcrtcs = outlist_it.value()->validCrtcs();
	out << "Valid crtcs:";
	for (int i=0; i<vcrtcs.count(); i++)
	    out << " " << vcrtcs[i];
	out << "\n";
	
	// Output Width & Height in mm
	out << "Size: " << outlist_it.value()->widthmm() << "x" << outlist_it.value()->heightmm() << " (in mm)\n";
	
	// Output Valid Rotations
	out << "Valid Rotations:";
	Rotation rots,rot;
	rots = outlist_it.value()->validRotations();
	rot = outlist_it.value()->currentRotation();
	if (rots & RR_Rotate_0)
	    out << " normal";
	if (rot & RR_Rotate_0)
	    out << "*";
	if (rots & RR_Rotate_90)
	    out << " left";
	if (rot & RR_Rotate_90)
	    out << "*";
	if (rots & RR_Rotate_180)
	    out << " inverted";
	if (rot & RR_Rotate_180)
	    out << "*";
	if (rots & RR_Rotate_270)
	    out << " right";
	if (rot & RR_Rotate_270)
	    out << "*";
	out << "\nValid Reflections:";
	if (rots & RR_Reflect_X)
	    out << " X-axis";
	if (rot & RR_Reflect_X)
	    out << "*";
	if (rots & RR_Reflect_Y)
	    out << " Y-axis";
	if (rot & RR_Reflect_Y)
	    out << "*";
	out << "\n";
    }
    
    return 0;
}
