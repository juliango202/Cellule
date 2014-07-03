import Cellule

from PyQt4 import Qt

import sip

def openScene():
    
    mainWinSIP = Cellule.toSIP( Cellule.appli.mainWin, Qt.QMainWindow )
    
    fileName = Qt.QFileDialog.getOpenFileName( mainWinSIP, 'Open Scene', '', 'Cellule scenes (*.cel)' )
    
    if not fileName.isEmpty():
        
        Cellule.appli.scene.load( Cellule.fromSIP( fileName ) )

def saveSceneAs():
    
    mainWinSIP = Cellule.toSIP( Cellule.appli.mainWin, Qt.QMainWindow )
    
    fileName = Qt.QFileDialog.getSaveFileName( mainWinSIP, 'Save Scene', '', 'Cellule scenes (*.cel)' )
    
    if not fileName.isEmpty():
        
        Cellule.appli.scene.saveAs( Cellule.fromSIP( fileName ) )

def saveImageAs( view2D ):
    
    mainWinSIP = Cellule.toSIP( Cellule.appli.mainWin, Qt.QMainWindow )
    
    fileName = Qt.QFileDialog.getSaveFileName( mainWinSIP, 'Save Rendered Image', '', 'Image files(*.png *.tiff *.bmp)' )
    
    if not fileName.isEmpty():
        
        view2D.renderArea.saveAs( Cellule.fromSIP( fileName ) )

def loadDynamicNode():
    
    mainWinSIP = Cellule.toSIP( Cellule.appli.mainWin, Qt.QMainWindow )
    
    fileName = Qt.QFileDialog.getOpenFileName( mainWinSIP, 'Load dynamic node', '', 'Dynamic nodes (*.cpp)' )
    
    if not fileName.isEmpty():
        
        Cellule.appli.scene.loadDynamicNode( Cellule.fromSIP( fileName ) )
