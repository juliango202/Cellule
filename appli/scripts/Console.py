import Cellule

from PyQt4 import Qt

class Widget( Qt.QWidget ):
    
    def sizeHint( self ):
        
        return Qt.QSize( 350, 250 )
    
    def __init__( self, parent ):
        
        Qt.QWidget.__init__( self, parent )
        
        #   Menus
        #
        menuBar = Qt.QMenuBar( self )
        
        fileMenu    = menuBar.addMenu( 'File'    )
        editMenu    = menuBar.addMenu( 'Edit'    )
        optionsMenu = menuBar.addMenu( 'Options' )
        
        openScriptAction    = fileMenu   .addAction( 'Open script...'    )
        saveScriptAction    = fileMenu   .addAction( 'Save script as...' )
        
        executeAction       = editMenu   .addAction( 'Execute'           )
        clearOutputAction   = editMenu   .addAction( 'Clear output'      )
        clearInputAction    = editMenu   .addAction( 'Clear input'       )
        clearAllAction      = editMenu   .addAction( 'Clear all'         )

        self.showStackTrace = optionsMenu.addAction( 'Show stack trace'  )
        
        self.showStackTrace.setCheckable( True  )
        self.showStackTrace.setChecked  ( False )

        openScriptAction   .setIcon     ( Qt.QIcon( 'icons/16x16/document-open.png'                     ) )
        saveScriptAction   .setIcon     ( Qt.QIcon( 'icons/16x16/document-save-as.png'                  ) )
        executeAction      .setIcon     ( Qt.QIcon( 'icons/16x16/go-jump-locationbar.png'               ) )
        clearOutputAction  .setIcon     ( Qt.QIcon( 'icons/16x16/edit-clear-locationbar-ltr.png'        ) )
        clearInputAction   .setIcon     ( Qt.QIcon( 'icons/16x16/edit-clear-locationbar-rtl.png'        ) )
        clearAllAction     .setIcon     ( Qt.QIcon( 'icons/16x16/trash-empty.png'                       ) )
        
        openScriptAction   .setStatusTip( 'Load a script file into the console'                           )
        saveScriptAction   .setStatusTip( 'Save the console input to a file'                              )
        executeAction      .setStatusTip( 'Execute console input selected code ( whole if no selection )' )
        clearOutputAction  .setStatusTip( 'Clear console output'                                          )
        clearInputAction   .setStatusTip( 'Clear console input'                                           )
        clearAllAction     .setStatusTip( 'Clear console input and output'                                )
        self.showStackTrace.setStatusTip( 'Detail error location when printing error messages'            )

        executeAction      .setShortcut ( Qt.QKeySequence( 'Ctrl+Return' ) )
        
        #   Output and Input
        #
        splitter = Qt.QSplitter( Qt.Qt.Vertical, self )
        
        self.output = Qt.QTextEdit( splitter )
        self.input  = InputWidget ( splitter )
        
        self.output.setReadOnly     ( True  )
        self.input.setAcceptRichText( False )
        
        #   Layout
        #
        layout = Qt.QVBoxLayout( self )
        
        layout.addWidget( menuBar  )
        layout.addWidget( splitter )
        
        #   Signal / Slots
        #
        logSIP          = Cellule.toSIP( Cellule.appli.log          )
        scriptEngineSIP = Cellule.toSIP( Cellule.appli.scriptEngine )
        
        Qt.QObject.connect( openScriptAction,  Qt.SIGNAL( 'triggered()' ), self.openScript )
        Qt.QObject.connect( saveScriptAction,  Qt.SIGNAL( 'triggered()' ), self.saveScript )
        
        Qt.QObject.connect( executeAction,     Qt.SIGNAL( 'triggered()' ), self.input.requestExecution        )
        Qt.QObject.connect( clearOutputAction, Qt.SIGNAL( 'triggered()' ), self.output,  Qt.SLOT( 'clear()' ) )
        Qt.QObject.connect( clearInputAction,  Qt.SIGNAL( 'triggered()' ), self.input,   Qt.SLOT( 'clear()' ) )
        Qt.QObject.connect( clearAllAction,    Qt.SIGNAL( 'triggered()' ), self.output,  Qt.SLOT( 'clear()' ) )
        Qt.QObject.connect( clearAllAction,    Qt.SIGNAL( 'triggered()' ), self.input,   Qt.SLOT( 'clear()' ) )
        
        Qt.QObject.connect( logSIP,     Qt.SIGNAL( 'htmlAdded(const QString&)' ), self.output,  Qt.SLOT( 'append(const QString&)' ) )
        Qt.QObject.connect( self.input, Qt.SIGNAL( 'executionRequested'        ), self.execute                                      )
    
    def openScript( self ):
        
        fileName = Qt.QFileDialog.getOpenFileName( self, 'Open Script', '', 'Python scripts (*.py)' )
        
        if not fileName.isEmpty():
            
            file = Qt.QFile( fileName )
            
            if file.open( Qt.QFile.ReadOnly | Qt.QFile.Text ):
                
                fileReader = Qt.QTextStream( file )
                self.input.document().setPlainText( fileReader.readAll() )
                
            else: Qt.QMessageBox.critical( self, 'Open Script', 'Could not open ' + fileName + ' for reading:\n' + file.errorString() )

    def saveScript( self ):
        
        fileName = Qt.QFileDialog.getSaveFileName( self, 'Save Script', '', 'Python scripts (*.py)' )
        
        if not fileName.isEmpty():
            
            file = Qt.QFile( fileName )
            
            if file.open( Qt.QFile.WriteOnly | Qt.QFile.Text ):
                
                Qt.QTextStream( file ) << self.input.document().toPlainText()
                
            else: Qt.QMessageBox.critical( self, 'Save Script', 'Could not open ' + fileName + ' for writing:\n' + file.errorString() )
    
    def execute( self, code ):
        
        code = unicode( code )
        
        import sys, traceback
        
        outStreamOrig = sys.stdout
        errStreamOrig = sys.stderr
        
        try:
            
            sys.stdout = TextWidgetStream( self.output, formatOutput )
            sys.stderr = TextWidgetStream( self.output, formatError  )
            
            sys.stdout.write( '<b>' + code + '</b>' )
            
            errorOccured = False
            
            try: result = eval( code )
            
            except SyntaxError:
                
                try: exec code in globals(), globals()
                
                except: errorOccured = True
                
            except: errorOccured = True
            
            else: sys.stdout.write( "# Result: %s" % result )
            
            if errorOccured:
                
                if self.showStackTrace.isChecked():
                    
                    errorMsgs = traceback.format_exception( sys.exc_info()[0], sys.exc_info()[1], sys.exc_info()[2] )
                    
                else: errorMsgs = traceback.format_exception_only( sys.exc_info()[0], sys.exc_info()[1] )
                
                sys.stderr.write( "".join( errorMsgs ) )
            
            elif not self.input.textCursor().hasSelection(): self.input.clear()

        finally:
                
            sys.stdout = outStreamOrig
            sys.stderr = errStreamOrig


class InputWidget( Qt.QTextEdit ):
    
    def keyPressEvent( self, event ):
        
        key          = event.key()
        modifiers    = event.modifiers()
        
        enterPressed      = ( key == Qt.Qt.Key_Enter  )
        ctrlReturnPressed = ( key == Qt.Qt.Key_Return ) and ( modifiers & Qt.Qt.ControlModifier )
        
        if enterPressed or ctrlReturnPressed: self.requestExecution()
        
        else: Qt.QTextEdit.keyPressEvent( self, event )
    
    #   Returns selected code or the whole if nothing is selected
    #
    def getCurrentCode( self ):
        
        textCursor = self.textCursor()
        
        if textCursor.hasSelection():   return textCursor.selection().toPlainText()
        else:                           return self.document().toPlainText()

    def requestExecution( self ):
        
        self.emit( Qt.SIGNAL( "executionRequested" ), self.getCurrentCode() )

def formatOutput( text ):

    span = [ '<span style="color: silver;">', '</span>' ]
    text = text.strip().replace( ' ', '&nbsp;' ).replace( '\n', '<br>' )
    return span[0] + text + span[1]

def formatError( text ):

    span = [ '<span style="color: red;">', '</span>' ]
    text = "#&nbsp;" + text.strip().replace( ' ', '&nbsp;' ).replace( '\n', '<br>#&nbsp;' )
    return span[0] + text + span[1]

class TextWidgetStream:
    
    def __init__( self, textWidget, formatMethod ):
        
        self.textWidget   = textWidget
        self.formatMethod = formatMethod
    
    def write( self, text ):
        
        self.textWidget.append( self.formatMethod( text ) )
