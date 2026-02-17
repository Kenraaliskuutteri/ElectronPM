#pip install PySide6
#This is so that you can more easily run the GUI! ^
import sys
from PySide6.QtWidgets import QApplication, QMainWindow, QWidget, QVBoxLayout, QLabel, QPushButton

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("ElectronPM Graphical Version")
        self.setGeometry(100, 100, 400, 300)
        
        widget = QWidget()
        self.setCentralWidget(widget)
        layout = QVBoxLayout(widget)
        
        self.label = QLabel("Hello, and welcome to the testing version - Kenraali :)")
        layout.addWidget(self.label)
        self.setGeometry(100, 100, 400, 300)
        
        button = QPushButton("Click for something funny :) (going to be editing this button soon as i update the GUI too!)")
        button.clicked.connect(self.on_click)
        layout.addWidget(button)
    
    def on_click(self):
        self.label.setText("Did you think that does something? Pffft, no it doesn't. This is just a test GUI!")

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec())
# This was pretty simple to do - Althought getting the right libraries killed me :( - Kenraali
