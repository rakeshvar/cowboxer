# Introduction #

Cowboxer is a boxing software tool for tesseract OCR. It is an improvement and customization of the existing [owlboxer](http://code.google.com/p/owlboxer/).



# Requirements #

  * Qt >= 4.5.1



# Installation #

```
qmake CowBoxer.pro
lupdate CowBoxer.pro
make
make install
```



# Features #

Cowboxer can handle string inputs to the 'character'. In many complex scripts a single image is composed of many characters

It has additional features like inserting/deleting ONLY a box/string so that the relative assignment of boxes to strings is shifted up or down.



# How to use Cowboxer #

## Keyboard shortcuts ##

| _**Shortcut**_ | _**Description**_ |
|:---------------|:------------------|
| Arrow key | Move box|
| Arrow key + Shift | Fast moving|
| Arrow key + Ctrl | Change box size|
| Arrow key + Ctrl + Shift | Fast box size changing|
| Return | Select next box|
| Shift + Return | Select previous box|
| Ctrl + Shift + Del | Delete current box along with the string|
| Ctrl + Del | Delete current box only (string transfers to next box)|
| Shift + Del | Delete current box's string only (next box's string transfers here)|
| Del | Clear current box's string|
| Ctrl + Shift + Ins | Insert new box & string (\"A\")|
| Ctrl + Ins | Insert new box only (Next box's string moves to this box)|
| Shift + Ins | Insert new string only (This string moves to next box)|
| Tab | Slice the current Box into sub boxes if possible and trim them. On loading a box file, boxes containing a \"~\" are sliced automatically.|
| Backspace | Backspace String|
| Any key | Input string for the current box|