import Cellule

import Actions

from PyQt4 import Qt

def init( mainWindow ):
    
    mainToolBar = mainWindow.addToolBar( "Tools" )

    mainToolBar.setIconSize( Qt.QSize( 32, 32 ) )
    
    for toolName in Cellule.appli.toolManager.toolNames:
        
        mainToolBar.addAction( Actions.mainWindow.tools[ toolName ] )
