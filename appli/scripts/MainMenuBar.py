import Cellule

import Actions

from PyQt4 import Qt

class Widget( Qt.QMenuBar ):

    def __init__( self, parent ):
        
        Qt.QMenuBar.__init__( self, parent )
        
        fileMenu          = self.addMenu( 'File'  )
        editMenu          = self.addMenu( 'Edit'  )
        nodesMenu         = self.addMenu( 'Nodes' )
        helpMenu          = self.addMenu( 'Help'  )
        
        fileMenu.addAction( Actions.mainWindow.newScene      )
        fileMenu.addAction( Actions.mainWindow.openScene     )
        fileMenu.addAction( Actions.mainWindow.saveScene     )
        fileMenu.addAction( Actions.mainWindow.saveSceneAs   )
        fileMenu.addAction( Actions.mainWindow.exit          )
        
        editMenu.addAction( Actions.mainWindow.randomFill    )
        
        for nodeTypeName in Cellule.appli.scene.nodeLibrary.typeNames:
            
            nodesMenu.addAction( Actions.mainWindow.nodes[ nodeTypeName ] )

        helpMenu.addAction( Actions.mainWindow.about         )
