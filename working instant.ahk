#Requires AutoHotkey v2.0
CapsLock::

{
MouseClick "right"
Sleep 500
SendMouse_RelativeMove(-600, 0)
Sleep 500
SendMouse_RelativeMove(600, 0)
sleep 0
MouseClick "left"
sleep 1
SendMouse_RelativeMove(600, 0)
Sleep 2000
SendMouse_RelativeMove(-600, 0)

}
SendMouse_RelativeMove(x, y)
{
    DllCall("mouse_event", "UInt", 0x01, "UInt", x, "UInt", y) ; move
}
return