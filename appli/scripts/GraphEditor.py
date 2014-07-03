import Cellule
import Actions

from PyQt4 import Qt

class Widget( Qt.QWidget ):

    #   Widget construction
    #
    def __init__( self, parent ):
        
        Qt.QWidget.__init__( self, parent )
        
        #   Actions
        #
        Actions.createGraphEditorActions( self )
        
        #   Menus
        #
        #menuBar = Qt.QMenuBar( self )
        #
        #fileMenu = menuBar.addMenu( 'File' )
        #viewMenu = menuBar.addMenu( 'View' )
        #
        #fileMenu.addAction( Actions.graphEditor.loadDynamicNode )
        #viewMenu.addAction( Actions.graphEditor.frameAll        )
        
        #   Graph editor
        #
        graphEditor    = Cellule.appli.createGraphEditor( Cellule.fromSIP( self ) )
        graphEditorSIP = Cellule.toSIP( graphEditor, Qt.QWidget )
        
        #   Layout
        #
        layout = Qt.QVBoxLayout( self )
        
        #layout.addWidget( menuBar        )
        layout.addWidget( graphEditorSIP )
