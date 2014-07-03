import CAapplication as appli

from PyQt4 import QtCore

import sip

def toSIP  ( obj, type = QtCore.QObject ): return sip.wrapinstance  ( obj.__toPointer__(), type )
def fromSIP( obj                        ): return sip.unwrapinstance( obj )
