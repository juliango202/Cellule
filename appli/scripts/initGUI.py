import Cellule

import Actions
import MainMenuBar
import MainToolBar
import View2D
import GraphEditor
import Console
import CommandLine

from PyQt4 import Qt

#   Get application main window as a SIP object
#
mainWinSIP = Cellule.toSIP( Cellule.appli.mainWin, Qt.QMainWindow )

Actions.createMainWindowActions( mainWinSIP )

#   Create menus
#
mainMenuBar = MainMenuBar.Widget( mainWinSIP )

mainWinSIP.setMenuBar( mainMenuBar )

#   Create main toolbar
#
MainToolBar.init( mainWinSIP )

#   Create 2D view
#
view2D = View2D.Widget( mainWinSIP )

mainWinSIP.setCentralWidget( view2D )

#   Create Graph Editor
#
graphEditorDock = Qt.QDockWidget( 'Graph Editor', mainWinSIP )
graphEditor     = GraphEditor.Widget( graphEditorDock )

graphEditorDock.setWidget( graphEditor )

mainWinSIP.addDockWidget( Qt.Qt.RightDockWidgetArea, graphEditorDock )

#   Create Node Editor
#
nodeEditorDock = Qt.QDockWidget( 'Node Editor', mainWinSIP )

nodeEditor     = Cellule.appli.createNodeEditor( Cellule.fromSIP( nodeEditorDock ) )
nodeEditorSIP  = Cellule.toSIP( nodeEditor, Qt.QWidget )

nodeEditorDock.setWidget( nodeEditorSIP )

mainWinSIP.addDockWidget( Qt.Qt.RightDockWidgetArea, nodeEditorDock )

#   Create console
#
consoleDock = Qt.QDockWidget( 'Console', mainWinSIP )
console     = Console.Widget( consoleDock )

consoleDock.setWidget( console )

mainWinSIP.addDockWidget( Qt.Qt.RightDockWidgetArea, consoleDock )

consoleDock.setFloating ( True  )
consoleDock.setVisible  ( False )

#   Create command line
#
commandLineDock = Qt.QDockWidget( 'Command Line', mainWinSIP ) 
commandLine     = CommandLine.Widget( commandLineDock )

commandLineDock.setWidget( commandLine )

commandLineDock.setAllowedAreas( Qt.Qt.BottomDockWidgetArea )

mainWinSIP.addDockWidget( Qt.Qt.BottomDockWidgetArea, commandLineDock )

#   Create status bar
#
statusBar = mainWinSIP.statusBar()
