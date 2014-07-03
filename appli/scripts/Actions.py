import Cellule

import FileBrowser

from PyQt4 import Qt


class Action( Qt.QAction ):

    def __init__( self, text, keySequence, description, iconName, parent ):
        
        Qt.QAction.__init__( self, parent )
        
        self.setText( text )
        
        if keySequence: self.setShortcut( Qt.QKeySequence( keySequence ) )
        
        self.setStatusTip( description )
        
        self.setIcon( iconName )
    
    def setIcon( self, iconName ):
        
        if iconName: Qt.QAction.setIcon( self, Qt.QIcon( 'icons/16x16/%s.png' % iconName ) )

class ToolAction( Action ):
    
    def __init__( self, name, keySequence, description, iconName, parent ):
        
        Action.__init__( self, name, keySequence, description, iconName, parent )
        
        Qt.QObject.connect( self, Qt.SIGNAL( 'triggered()' ), self.setAsCurrent )
        
    def setIcon( self, iconName ):
        
        if iconName: Qt.QAction.setIcon( self, Qt.QIcon( 'icons/32x32/%s.png' % iconName ) )

    def setAsCurrent( self ):
        
        toolName = self.text()
        Cellule.appli.toolManager.setCurrentTool( Cellule.fromSIP( toolName ) )

class NodeAction( Action ):
    
    def __init__( self, typeName, keySequence, description, iconName, parent ):
        
        Action.__init__( self, typeName, keySequence, description, iconName, parent )
        
        Qt.QObject.connect( self, Qt.SIGNAL( 'triggered()' ), self.create )
        
    def setIcon( self, iconName ):
        
        if iconName: Qt.QAction.setIcon( self, Qt.QIcon( 'icons/32x32/%s.png' % iconName ) )

    def create( self ):
        
        nodeTypeName = self.text()
        Cellule.appli.scene.createNode( Cellule.fromSIP( nodeTypeName ) )

#################################################################################################################################################
#
#   Main Window Actions
#
class MainWindow: pass
mainWindow = MainWindow

def createMainWindowActions( mainWindowSIP ):
    
    #   Tools
    #
    mainWindow.tools = {}

    mainWindow.tools[ "Emitter" ]       = ToolAction( 'Emitter',            'E',        'Add cells',                                  None,                mainWindowSIP )
    mainWindow.tools[ "Eraser"  ]       = ToolAction( 'Eraser',             'Del',      'Remove cells',                               None,                mainWindowSIP )
    mainWindow.tools[ "Size"    ]       = ToolAction( 'Size',               'S',        'Resize cells',                               None,                mainWindowSIP )
    mainWindow.tools[ "Magneto" ]       = ToolAction( 'Magneto',            'M',        'Repulse ( or attract ) cells',               None,                mainWindowSIP )
    mainWindow.tools[ "Relax"   ]       = ToolAction( 'Relax',              'X',        'Smoothly organize cells',                    None,                mainWindowSIP )
    mainWindow.tools[ "Render"  ]       = ToolAction( 'Render',             'R',        'Select and render an area',                  None,                mainWindowSIP )
    mainWindow.tools[ "Color"   ]       = ToolAction( 'Color',              'C',        'Colorize cells',                             None,                mainWindowSIP )
    mainWindow.tools[ "Shader"  ]       = ToolAction( 'Shader',             'S',        'Assign selected shader to cells',            None,                mainWindowSIP )
    
    #   Nodes
    #
    mainWindow.nodes = {}
    
    mainWindow.nodes[ "renderSettings" ] = NodeAction( 'renderSettings',      None,      'Renderer settings',                                   None,       mainWindowSIP )
    mainWindow.nodes[ "cellSampler"    ] = NodeAction( 'cellSampler',         None,      'Output basic cell geometry informations',             None,       mainWindowSIP )
    mainWindow.nodes[ "surfaceShader"  ] = NodeAction( 'surfaceShader',       None,      'Output shading informations to the assigned surface', None,       mainWindowSIP )
    mainWindow.nodes[ "colorGradient"  ] = NodeAction( 'colorGradient',      'G',        'Ouput interpolation between input colors',            None,       mainWindowSIP )
    mainWindow.nodes[ "decomposeColor" ] = NodeAction( 'decomposeColor',      None,      'Ouput hue, saturation and value of an input color',   None,       mainWindowSIP )
    mainWindow.nodes[ "composeReals"   ] = NodeAction( 'composeReals',        None,      'Multiply, divide, add or substract two floats',       None,       mainWindowSIP )
    mainWindow.nodes[ "composeColors"  ] = NodeAction( 'composeColors',       None,      'Blend, add or substract two colors',                  None,       mainWindowSIP )
    mainWindow.nodes[ "randomizer"     ] = NodeAction( 'randomizer',          None,      'Random number generator',                             None,       mainWindowSIP )


    #   Commands
    #
    mainWindow.newScene                 = Action    ( 'New scene',                 'Ctrl+N',   'Clear the current scene',                   'document-new',       mainWindowSIP )
    mainWindow.openScene                = Action    ( 'Open scene...',             'Ctrl+O',   'Open a scene',                              'document-open',      mainWindowSIP )
    mainWindow.saveScene                = Action    ( 'Save scene',                'Ctrl+S',   'Save current scene',                        'document-save',      mainWindowSIP )
    mainWindow.saveSceneAs              = Action    ( 'Save scene as...',           None,      'Save current scene under a new name',       'document-save-as',   mainWindowSIP )
    mainWindow.exit                     = Action    ( 'Exit',                      'Ctrl+Q',   'Exit Cellule',                              'application-exit',   mainWindowSIP )
    
    mainWindow.randomFill               = Action    ( 'Fill randomly  ',           'F',        'Fill the unit square with random cells',     None,                mainWindowSIP )
    
    mainWindow.about                    = Action    ( 'About...',                  None,      'Displays version number, license, credits', 'cellule',            mainWindowSIP )

    #   Connections
    #
    scene = Cellule.appli.scene
    
    Qt.QObject.connect( mainWindow.newScene,      Qt.SIGNAL( 'triggered()' ), scene.clear               )
    Qt.QObject.connect( mainWindow.openScene,     Qt.SIGNAL( 'triggered()' ), FileBrowser.openScene     )
    Qt.QObject.connect( mainWindow.saveScene,     Qt.SIGNAL( 'triggered()' ), scene.save                )
    Qt.QObject.connect( mainWindow.saveSceneAs,   Qt.SIGNAL( 'triggered()' ), FileBrowser.saveSceneAs   )
    Qt.QObject.connect( mainWindow.exit,          Qt.SIGNAL( 'triggered()' ), Cellule.appli.quit        )
    
    Qt.QObject.connect( mainWindow.randomFill,    Qt.SIGNAL( 'triggered()' ), scene.fillRandomly        )

    Qt.QObject.connect( mainWindow.about,         Qt.SIGNAL( 'triggered()' ), Cellule.appli.about       )

#################################################################################################################################################
#
#   Graph Editor Actions
#
class GraphEditor: pass
graphEditor = GraphEditor

def createGraphEditorActions( graphEditorSIP ):

    graphEditor.loadDynamicNode = Action( 'Load dynamic node',    None,      'Load a dynamic node',               None,     graphEditorSIP )
    graphEditor.frameAll        = Action( 'Frame all  ',         'A',        'Center view on the whole graph',    None,     graphEditorSIP )
    
    Qt.QObject.connect( graphEditor.loadDynamicNode, Qt.SIGNAL( 'triggered()' ), FileBrowser.loadDynamicNode )
