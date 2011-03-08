#include <QTextStream>
#include "configuration.h"

int main(int argc, char **argv) {
    QTextStream out(stdout);
    out << "Hello, world!\n";
    EasyRandR::Configuration cfg;
    
    QMap<RROutput,EasyRandR::Output*> outlist;
    outlist = cfg.getOutputs();
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
	
	// Output Clones
	QList<RROutput> clones;
	clones = outlist_it.value()->clones();
	out << "Clones:";
	for (int i=0; i<clones.count(); i++)
	    out << " " << outlist.value(clones[i])->name();
	out << "\n";
	
	// Output Connection & Crtc
	if (outlist_it.value()->connectionStatus() == RR_Connected)
	    out << "Connected to CRTC" << outlist_it.value()->crtc() << "\n";
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
    }
    
    return 0;
}
