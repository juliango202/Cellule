import Cellule
import FileBrowser

from PyQt4 import Qt

class Widget( Qt.QWidget ):

    #   Widget construction
    #
    def __init__( self, parent ):
        
        Qt.QWidget.__init__( self, parent )
        
        menuBar = Qt.QMenuBar( self )
        
        #   Display menu
        #
        displayMenu = menuBar.addMenu( 'Display' )
        
        displayLevelGroup           = Qt.QActionGroup( displayMenu )
        
        displayLevelPointAction     = Qt.QAction( 'Point',     displayLevelGroup  )
        displayLevelWireframeAction = Qt.QAction( 'Wireframe', displayLevelGroup  )
        displayLevelBSplineAction   = Qt.QAction( 'BSpline',   displayLevelGroup  )
        displayLevelShadedAction    = Qt.QAction( 'Shaded',    displayLevelGroup  )

        displayLevelPointAction.setCheckable    ( True )
        displayLevelWireframeAction.setCheckable( True )
        displayLevelBSplineAction.setCheckable  ( True )
        displayLevelShadedAction.setCheckable   ( True )
        
        displayLevelShadedAction.setChecked     ( True )
        
        showSeedsAction             = Qt.QAction( 'Show seeds',    displayMenu  )
        showDelauneyAction          = Qt.QAction( 'Show Delauney', displayMenu  )
        
        showSeedsAction.setCheckable   ( True )
        showDelauneyAction.setCheckable( True )
        
        displayMenu.addAction( displayLevelPointAction     )
        displayMenu.addAction( displayLevelWireframeAction )
        displayMenu.addAction( displayLevelBSplineAction   )
        displayMenu.addAction( displayLevelShadedAction    )
        displayMenu.addAction( showSeedsAction             )
        displayMenu.addAction( showDelauneyAction          )
        
        #   Render menu
        #
        renderMenu = menuBar.addMenu( 'Render' )
        
        showRenderSettingsAction  = Qt.QAction( 'Settings...',      renderMenu )
        saveRenderedImageAsAction = Qt.QAction( 'Save image as...', renderMenu )
        
        #renderMenu.addAction( showRenderSettingsAction  )
        renderMenu.addAction( saveRenderedImageAsAction )
        
        #   2D View
        #
        self.view2D = Cellule.appli.createView2D( Cellule.fromSIP( self ) )
        view2DSIP   = Cellule.toSIP( self.view2D, Qt.QWidget )
        
        #   Layout
        #
        layout = Qt.QVBoxLayout( self )
        
        layout.addWidget( menuBar   )
        layout.addWidget( view2DSIP )

        #   Signals / Slots
        #
        Qt.QObject.connect( displayLevelPointAction,     Qt.SIGNAL( 'triggered()'   ), self.view2D.setDisplayLevelPoint     )
        Qt.QObject.connect( displayLevelWireframeAction, Qt.SIGNAL( 'triggered()'   ), self.view2D.setDisplayLevelWireframe )
        Qt.QObject.connect( displayLevelShadedAction,    Qt.SIGNAL( 'triggered()'   ), self.view2D.setDisplayLevelShaded    )
        Qt.QObject.connect( displayLevelBSplineAction,   Qt.SIGNAL( 'triggered()'   ), self.view2D.setDisplayLevelBSpline   )
        
        Qt.QObject.connect( showSeedsAction,             Qt.SIGNAL( 'toggled(bool)' ), self.view2D.displaySeeds    )
        Qt.QObject.connect( showDelauneyAction,          Qt.SIGNAL( 'toggled(bool)' ), self.view2D.displayDelauney )
        
        Qt.QObject.connect( showRenderSettingsAction,    Qt.SIGNAL( 'triggered()'   ), self.showRenderSettings  )
        Qt.QObject.connect( saveRenderedImageAsAction,   Qt.SIGNAL( 'triggered()'   ), self.saveRenderedImageAs )

    def showRenderSettings( self ):
        
        pass

    def saveRenderedImageAs( self ):
        
        FileBrowser.saveImageAs( self.view2D )
