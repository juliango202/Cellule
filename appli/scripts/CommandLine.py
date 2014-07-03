import Cellule

from PyQt4 import Qt

class Widget( Qt.QSplitter ):

    def __init__( self, parent ):
        
        #   Create an input command line and an output line
        #   separated by an adjustable splitter
        #
        Qt.QSplitter.__init__( self, parent )
        
        inputLine  = Qt.QLineEdit( self )
        outputLine = Qt.QLineEdit( self )
        
        #   Configure the widgets
        #
        self.setSizePolicy( Qt.QSizePolicy.Expanding, Qt.QSizePolicy.Fixed )

        inputLine.palette().setColor( Qt.QPalette.Base, Qt.QColor( 200, 200, 200 ) )
        inputLine.palette().setColor( Qt.QPalette.Text, Qt.QColor( 0,   0,   0   ) )

        outputLine.setReadOnly( True )

        #   Signals / Slots
        #
        logSIP = Cellule.toSIP( Cellule.appli.log )

        Qt.QObject.connect( logSIP, Qt.SIGNAL( 'textAdded(const QString&)' ), outputLine, Qt.SLOT( 'setText(const QString&)' ) )
