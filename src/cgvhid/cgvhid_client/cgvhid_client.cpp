/*
 * Copyright 2020-present Ksyun
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "cgvhid_client.h"

#include <mutex>

#include "cgvhid.h"

namespace {
uint8_t keymap[255];
std::once_flag keymap_initialized;

// clang-format off
void KeyboardMappingInit() {
  keymap[VK_LBUTTON]                         = KEY_NONE;              // 0x01 Left mouse button
  keymap[VK_RBUTTON]                         = KEY_NONE;              // 0x02 Right mouse button
  keymap[VK_CANCEL]                          = KEY_NONE;              // 0x03 Control-break processing
  keymap[VK_MBUTTON]                         = KEY_NONE;              // 0x04 Middle mouse button
  keymap[VK_XBUTTON1]                        = KEY_NONE;              // 0x05 X1 mouse button
  keymap[VK_XBUTTON2]                        = KEY_NONE;              // 0x06 X2 mouse button
  keymap[0x07]                               = KEY_NONE;              // 0x07 reserved
  keymap[VK_BACK]                            = KEY_BACKSPACE;         // 0x08 BACKSPACE key
  keymap[VK_TAB]                             = KEY_TAB;               // 0x09 TAB key
  keymap[0x0A]                               = KEY_NONE;              // 0x0A-0x0B Reserved
  keymap[0x0B]                               = KEY_NONE;              // 0x0A-0x0B Reserved
  keymap[VK_CLEAR]                           = KEY_NONE;              // 0x0C CLEAR key, Num lock off, keypad 5.
  keymap[VK_RETURN]                          = KEY_ENTER;             // 0x0D ENTER key
  keymap[0x0E]                               = KEY_NONE;              // 0x0E-0x0F Undefined
  keymap[0x0F]                               = KEY_NONE;              // 0x0E-0x0F Undefined
  keymap[VK_SHIFT]                           = KEY_NONE;              // 0x10 SHIFT key
  keymap[VK_CONTROL]                         = KEY_NONE;              // 0x11 CTRL key
  keymap[VK_MENU]                            = KEY_NONE;              // 0x12 ALT key
  keymap[VK_PAUSE]                           = KEY_PAUSE;             // 0x13 PAUSE key
  keymap[VK_CAPITAL]                         = KEY_CAPSLOCK;          // 0x14 CAPS LOCK key
  keymap[VK_HANGUL]                          = KEY_HANGEUL;           // 0x15 IME Hangul mode
  keymap[0x16]                               = KEY_NONE;              // 0x16 IME On
  keymap[VK_JUNJA]                           = KEY_NONE;              // 0x17 IME Junja mode
  keymap[VK_FINAL]                           = KEY_NONE;              // 0x18 IME final mode
  keymap[VK_HANJA]                           = KEY_HANJA;             // 0x19 IME Hanja mode
  keymap[VK_KANJI]                           = KEY_NONE;              // 0x19 IME Kanji mode
  keymap[0x1A]                               = KEY_NONE;              // 0x1A unassigned
  keymap[VK_ESCAPE]                          = KEY_ESC;               // 0x1B ESC key
  keymap[VK_CONVERT]                         = KEY_NONE;              // 0x1C IME convert
  keymap[VK_NONCONVERT]                      = KEY_NONE;              // 0x1D IME nonconvert
  keymap[VK_ACCEPT]                          = KEY_NONE;              // 0x1E IME accept
  keymap[VK_MODECHANGE]                      = KEY_NONE;              // 0x1F IME mode change request
  keymap[VK_SPACE]                           = KEY_SPACE;             // 0x20 SPACEBAR
  keymap[VK_PRIOR]                           = KEY_PAGEUP;            // 0x21 PAGE UP key
  keymap[VK_NEXT]                            = KEY_PAGEDOWN;          // 0x22 PAGE DOWN key
  keymap[VK_END]                             = KEY_END;               // 0x23 END key
  keymap[VK_HOME]                            = KEY_HOME;              // 0x24 HOME key
  keymap[VK_LEFT]                            = KEY_LEFT;              // 0x25 LEFT ARROW key
  keymap[VK_UP]                              = KEY_UP;                // 0x26 UP ARROW key
  keymap[VK_RIGHT]                           = KEY_RIGHT;             // 0x27 RIGHT ARROW key
  keymap[VK_DOWN]                            = KEY_DOWN;              // 0x28 DOWN ARROW key
  keymap[VK_SELECT]                          = KEY_NONE;              // 0x29 SELECT key
  keymap[VK_PRINT]                           = KEY_NONE;              // 0x2A PRINT key
  keymap[VK_EXECUTE]                         = KEY_OPEN;              // 0x2B EXECUTE key
  keymap[VK_SNAPSHOT]                        = KEY_SYSRQ;             // 0x2C PRINT SCREEN key
  keymap[VK_INSERT]                          = KEY_INSERT;            // 0x2D INS key
  keymap[VK_DELETE]                          = KEY_DELETE;            // 0x2E DEL key
  keymap[VK_HELP]                            = KEY_HELP;              // 0x2F HELP key
  keymap[0x30]                               = KEY_0;                 // 0x30 0 key
  keymap[0x31]                               = KEY_1;                 // 0x31 1 key
  keymap[0x32]                               = KEY_2;                 // 0x32 2 key
  keymap[0x33]                               = KEY_3;                 // 0x33 3 key
  keymap[0x34]                               = KEY_4;                 // 0x34 4 key
  keymap[0x35]                               = KEY_5;                 // 0x35 5 key
  keymap[0x36]                               = KEY_6;                 // 0x36 6 key
  keymap[0x37]                               = KEY_7;                 // 0x37 7 key
  keymap[0x38]                               = KEY_8;                 // 0x38 8 key
  keymap[0x39]                               = KEY_9;                 // 0x39 9 key
  keymap[0x3A]                               = KEY_NONE;              // 0x3A-0x40 unassigned
  keymap[0x3B]                               = KEY_NONE;              // 0x3A-0x40 unassigned
  keymap[0x3C]                               = KEY_NONE;              // 0x3A-0x40 unassigned
  keymap[0x3D]                               = KEY_NONE;              // 0x3A-0x40 unassigned
  keymap[0x3E]                               = KEY_NONE;              // 0x3A-0x40 unassigned
  keymap[0x3F]                               = KEY_NONE;              // 0x3A-0x40 unassigned
  keymap[0x40]                               = KEY_NONE;              // 0x3A-0x40 unassigned
  keymap[0x41]                               = KEY_A;                 // 0x41 A key
  keymap[0x42]                               = KEY_B;                 // 0x42 B key
  keymap[0x43]                               = KEY_C;                 // 0x43 C key
  keymap[0x44]                               = KEY_D;                 // 0x44 D key
  keymap[0x45]                               = KEY_E;                 // 0x45 E key
  keymap[0x46]                               = KEY_F;                 // 0x46 F key
  keymap[0x47]                               = KEY_G;                 // 0x47 G key
  keymap[0x48]                               = KEY_H;                 // 0x48 H key
  keymap[0x49]                               = KEY_I;                 // 0x49 I key
  keymap[0x4A]                               = KEY_J;                 // 0x4A J key
  keymap[0x4B]                               = KEY_K;                 // 0x4B K key
  keymap[0x4C]                               = KEY_L;                 // 0x4C L key
  keymap[0x4D]                               = KEY_M;                 // 0x4D M key
  keymap[0x4E]                               = KEY_N;                 // 0x4E N key
  keymap[0x4F]                               = KEY_O;                 // 0x4F O key
  keymap[0x50]                               = KEY_P;                 // 0x50 P key
  keymap[0x51]                               = KEY_Q;                 // 0x51 Q key
  keymap[0x52]                               = KEY_R;                 // 0x52 R key
  keymap[0x53]                               = KEY_S;                 // 0x53 S key
  keymap[0x54]                               = KEY_T;                 // 0x54 T key
  keymap[0x55]                               = KEY_U;                 // 0x55 U ke+A78:C80y
  keymap[0x56]                               = KEY_V;                 // 0x56 V key
  keymap[0x57]                               = KEY_W;                 // 0x57 W key
  keymap[0x58]                               = KEY_X;                 // 0x58 X key
  keymap[0x59]                               = KEY_Y;                 // 0x59 Y key
  keymap[0x5A]                               = KEY_Z;                 // 0x5A Z key
  keymap[VK_LWIN]                            = KEY_LEFTMETA;          // 0x5B Left Windows key (Natural keyboard)
  keymap[VK_RWIN]                            = KEY_RIGHTMETA;         // 0x5C Right Windows key (Natural keyboard)
  keymap[VK_APPS]                            = KEY_COMPOSE;           // 0x5D Applications key (Natural keyboard)
  keymap[0x5E]                               = KEY_NONE;              // 0x5E Reserved
  keymap[VK_SLEEP]                           = KEY_MEDIA_SLEEP;       // 0x5F Computer Sleep key
  keymap[VK_NUMPAD0]                         = KEY_KP0;               // 0x60 Numeric keypad 0 key
  keymap[VK_NUMPAD1]                         = KEY_KP1;               // 0x61 Numeric keypad 1 key
  keymap[VK_NUMPAD2]                         = KEY_KP2;               // 0x62 Numeric keypad 2 key
  keymap[VK_NUMPAD3]                         = KEY_KP3;               // 0x63 Numeric keypad 3 key
  keymap[VK_NUMPAD4]                         = KEY_KP4;               // 0x64 Numeric keypad 4 key
  keymap[VK_NUMPAD5]                         = KEY_KP5;               // 0x65 Numeric keypad 5 key
  keymap[VK_NUMPAD6]                         = KEY_KP6;               // 0x66 Numeric keypad 6 key
  keymap[VK_NUMPAD7]                         = KEY_KP7;               // 0x67 Numeric keypad 7 key
  keymap[VK_NUMPAD8]                         = KEY_KP8;               // 0x68 Numeric keypad 8 key
  keymap[VK_NUMPAD9]                         = KEY_KP9;               // 0x69 Numeric keypad 9 key
  keymap[VK_MULTIPLY]                        = KEY_KPASTERISK;        // 0x6A Multiply key
  keymap[VK_ADD]                             = KEY_KPPLUS;            // 0x6B Add key
  keymap[VK_SEPARATOR]                       = KEY_KPENTER;           // 0x6C Separator key
  keymap[VK_SUBTRACT]                        = KEY_KPMINUS;           // 0x6D Subtract key
  keymap[VK_DECIMAL]                         = KEY_KPDOT;             // 0x6E Decimal key
  keymap[VK_DIVIDE]                          = KEY_KPSLASH;           // 0x6F Divide key
  keymap[VK_F1]                              = KEY_F1;                // 0x70 F1 key
  keymap[VK_F2]                              = KEY_F2;                // 0x71 F2 key
  keymap[VK_F3]                              = KEY_F3;                // 0x72 F3 key
  keymap[VK_F4]                              = KEY_F4;                // 0x73 F4 key
  keymap[VK_F5]                              = KEY_F5;                // 0x74 F5 key
  keymap[VK_F6]                              = KEY_F6;                // 0x75 F6 key
  keymap[VK_F7]                              = KEY_F7;                // 0x76 F7 key
  keymap[VK_F8]                              = KEY_F8;                // 0x77 F8 key
  keymap[VK_F9]                              = KEY_F9;                // 0x78 F9 key
  keymap[VK_F10]                             = KEY_F10;               // 0x79 F10 key
  keymap[VK_F11]                             = KEY_F11;               // 0x7A F11 key
  keymap[VK_F12]                             = KEY_F12;               // 0x7B F12 key
  keymap[VK_F13]                             = KEY_F13;               // 0x7C F13 key
  keymap[VK_F14]                             = KEY_F14;               // 0x7D F14 key
  keymap[VK_F15]                             = KEY_F15;               // 0x7E F15 key
  keymap[VK_F16]                             = KEY_F16;               // 0x7F F16 key
  keymap[VK_F17]                             = KEY_F17;               // 0x80 F17 key
  keymap[VK_F18]                             = KEY_F18;               // 0x81 F18 key
  keymap[VK_F19]                             = KEY_F19;               // 0x82 F19 key
  keymap[VK_F20]                             = KEY_F20;               // 0x83 F20 key
  keymap[VK_F21]                             = KEY_F21;               // 0x84 F21 key
  keymap[VK_F22]                             = KEY_F22;               // 0x85 F22 key
  keymap[VK_F23]                             = KEY_F23;               // 0x86 F23 key
  keymap[VK_F24]                             = KEY_F24;               // 0x87 F24 key
  keymap[VK_NAVIGATION_VIEW]                 = KEY_NONE;              // 0x88 UI navigation, reserved
  keymap[VK_NAVIGATION_MENU]                 = KEY_NONE;              // 0x89 UI navigation, reserved
  keymap[VK_NAVIGATION_UP]                   = KEY_NONE;              // 0x8A UI navigation, reserved
  keymap[VK_NAVIGATION_DOWN]                 = KEY_NONE;              // 0x8B UI navigation, reserved
  keymap[VK_NAVIGATION_LEFT]                 = KEY_NONE;              // 0x8C UI navigation, reserved
  keymap[VK_NAVIGATION_RIGHT]                = KEY_NONE;              // 0x8D UI navigation, reserved
  keymap[VK_NAVIGATION_ACCEPT]               = KEY_NONE;              // 0x8E UI navigation, reserved
  keymap[VK_NAVIGATION_CANCEL]               = KEY_NONE;              // 0x8F UI navigation, reserved
  keymap[VK_NUMLOCK]                         = KEY_NUMLOCK;           // 0x90 NUM LOCK key
  keymap[VK_SCROLL]                          = KEY_SCROLLLOCK;        // 0x91 SCROLL LOCK key
  keymap[VK_OEM_FJ_JISHO]                    = KEY_NONE;              // 0x92
  keymap[VK_OEM_FJ_MASSHOU]                  = KEY_NONE;              // 0x93
  keymap[VK_OEM_FJ_TOUROKU]                  = KEY_NONE;              // 0x94
  keymap[VK_OEM_FJ_LOYA]                     = KEY_NONE;              // 0x95
  keymap[VK_OEM_FJ_ROYA]                     = KEY_NONE;              // 0x96
  keymap[0x97]                               = KEY_NONE;              // 0x97-9F Unassigned
  keymap[0x98]                               = KEY_NONE;              // 0x97-9F Unassigned
  keymap[0x99]                               = KEY_NONE;              // 0x97-9F Unassigned
  keymap[0x9A]                               = KEY_NONE;              // 0x97-9F Unassigned
  keymap[0x9B]                               = KEY_NONE;              // 0x97-9F Unassigned
  keymap[0x9C]                               = KEY_NONE;              // 0x97-9F Unassigned
  keymap[0x9D]                               = KEY_NONE;              // 0x97-9F Unassigned
  keymap[0x9E]                               = KEY_NONE;              // 0x97-9F Unassigned
  keymap[0x9F]                               = KEY_NONE;              // 0x97-9F Unassigned
  keymap[VK_LSHIFT]                          = KEY_LEFTSHIFT;         // 0xA0 Left SHIFT key
  keymap[VK_RSHIFT]                          = KEY_RIGHTSHIFT;        // 0xA1 Right SHIFT key
  keymap[VK_LCONTROL]                        = KEY_LEFTCTRL;          // 0xA2 Left CONTROL key
  keymap[VK_RCONTROL]                        = KEY_RIGHTCTRL;         // 0xA3 Right CONTROL key
  keymap[VK_LMENU]                           = KEY_LEFTALT;           // 0xA4 Left MENU key
  keymap[VK_RMENU]                           = KEY_RIGHTALT;          // 0xA5 Right MENU key
  keymap[VK_BROWSER_BACK]                    = KEY_MEDIA_BACK;        // 0xA6 Browser Back key
  keymap[VK_BROWSER_FORWARD]                 = KEY_MEDIA_FORWARD;     // 0xA7 Browser Forward key
  keymap[VK_BROWSER_REFRESH]                 = KEY_MEDIA_REFRESH;     // 0xA8 Browser Refresh key
  keymap[VK_BROWSER_STOP]                    = KEY_MEDIA_STOP;        // 0xA9 Browser Stop key
  keymap[VK_BROWSER_SEARCH]                  = KEY_MEDIA_FIND;        // 0xAA Browser Search key
  keymap[VK_BROWSER_FAVORITES]               = KEY_MEDIA_COFFEE;      // 0xAB Browser Favorites key
  keymap[VK_BROWSER_HOME]                    = KEY_MEDIA_WWW;         // 0xAC Browser Start and Home key
  keymap[VK_VOLUME_MUTE]                     = KEY_MEDIA_MUTE;        // 0xAD Volume Mute key
  keymap[VK_VOLUME_DOWN]                     = KEY_MEDIA_VOLUMEDOWN;  // 0xAE Volume Down key
  keymap[VK_VOLUME_UP]                       = KEY_MEDIA_VOLUMEUP;    // 0xAF Volume Up key
  keymap[VK_MEDIA_NEXT_TRACK]                = KEY_MEDIA_NEXTSONG;    // 0xB0 Next Track key
  keymap[VK_MEDIA_PREV_TRACK]                = KEY_MEDIA_PREVIOUSSONG;// 0xB1 Previous Track key
  keymap[VK_MEDIA_STOP]                      = KEY_MEDIA_STOPCD;      // 0xB2 Stop Media key
  keymap[VK_MEDIA_PLAY_PAUSE]                = KEY_MEDIA_PLAYPAUSE;   // 0xB3 Play/Pause Media key
  keymap[VK_LAUNCH_MAIL]                     = KEY_NONE;              // 0xB4 Start Mail key
  keymap[VK_LAUNCH_MEDIA_SELECT]             = KEY_NONE;              // 0xB5 Select Media key
  keymap[VK_LAUNCH_APP1]                     = KEY_NONE;              // 0xB6 Start Application 1 key
  keymap[VK_LAUNCH_APP2]                     = KEY_NONE;              // 0xB7 Start Application 2 key
  keymap[0xB8]                               = KEY_NONE;              // 0xB8-B9 Reserved
  keymap[0xB9]                               = KEY_NONE;              // 0xB8-B9 Reserved
  keymap[VK_OEM_1]                           = KEY_SEMICOLON;         // 0xBA Used for miscellaneous characters; it can vary by keyboard.
  keymap[VK_OEM_PLUS]                        = KEY_EQUAL;             // 0xBB For any country/region, the '+' key
  keymap[VK_OEM_COMMA]                       = KEY_COMMA;             // 0xBC For any country/region, the ',' key
  keymap[VK_OEM_MINUS]                       = KEY_MINUS;             // 0xBD For any country/region, the '-' key
  keymap[VK_OEM_PERIOD]                      = KEY_DOT;               // 0xBE For any country/region, the '.' key
  keymap[VK_OEM_2]                           = KEY_SLASH;             // 0xBF "Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '/?' key"
  keymap[VK_OEM_3]                           = KEY_GRAVE;             // 0xC0 "Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '`~' key"
  keymap[0xC1]                               = KEY_NONE;              // 0xC1-C2 Reserved
  keymap[0xC2]                               = KEY_NONE;              // 0xC1-C2 Reserved
  keymap[VK_GAMEPAD_A]                       = KEY_NONE;              // 0xC3 Gamepad input
  keymap[VK_GAMEPAD_B]                       = KEY_NONE;              // 0xC4 Gamepad input
  keymap[VK_GAMEPAD_X]                       = KEY_NONE;              // 0xC5 Gamepad input
  keymap[VK_GAMEPAD_Y]                       = KEY_NONE;              // 0xC6 Gamepad input
  keymap[VK_GAMEPAD_RIGHT_SHOULDER]          = KEY_NONE;              // 0xC7 Gamepad input
  keymap[VK_GAMEPAD_LEFT_SHOULDER]           = KEY_NONE;              // 0xC8 Gamepad input
  keymap[VK_GAMEPAD_LEFT_TRIGGER]            = KEY_NONE;              // 0xC9 Gamepad input
  keymap[VK_GAMEPAD_RIGHT_TRIGGER]           = KEY_NONE;              // 0xCA Gamepad input
  keymap[VK_GAMEPAD_DPAD_UP]                 = KEY_NONE;              // 0xCB Gamepad input
  keymap[VK_GAMEPAD_DPAD_DOWN]               = KEY_NONE;              // 0xCC Gamepad input
  keymap[VK_GAMEPAD_DPAD_LEFT]               = KEY_NONE;              // 0xCD Gamepad input
  keymap[VK_GAMEPAD_DPAD_RIGHT]              = KEY_NONE;              // 0xCE Gamepad input
  keymap[VK_GAMEPAD_MENU]                    = KEY_NONE;              // 0xCF Gamepad input
  keymap[VK_GAMEPAD_VIEW]                    = KEY_NONE;              // 0xD0 Gamepad input
  keymap[VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON]  = KEY_NONE;              // 0xD1 Gamepad input
  keymap[VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON] = KEY_NONE;              // 0xD2 Gamepad input
  keymap[VK_GAMEPAD_LEFT_THUMBSTICK_UP]      = KEY_NONE;              // 0xD3 Gamepad input
  keymap[VK_GAMEPAD_LEFT_THUMBSTICK_DOWN]    = KEY_NONE;              // 0xD4 Gamepad input
  keymap[VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT]   = KEY_NONE;              // 0xD5 Gamepad input
  keymap[VK_GAMEPAD_LEFT_THUMBSTICK_LEFT]    = KEY_NONE;              // 0xD6 Gamepad input
  keymap[VK_GAMEPAD_RIGHT_THUMBSTICK_UP]     = KEY_NONE;              // 0xD7 Gamepad input
  keymap[VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN]   = KEY_NONE;              // 0xD8 Gamepad input
  keymap[VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT]  = KEY_NONE;              // 0xD9 Gamepad input
  keymap[VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT]   = KEY_NONE;              // 0xDA Gamepad input
  keymap[VK_OEM_4]                           = KEY_LEFTBRACE;         // 0xDB "Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '[{' key"
  keymap[VK_OEM_5]                           = KEY_BACKSLASH;         // 0xDC "Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '\|' key"
  keymap[VK_OEM_6]                           = KEY_RIGHTBRACE;        // 0xDD "Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ']}' key"
  keymap[VK_OEM_7]                           = KEY_APOSTROPHE;        // 0xDE "Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the 'single-quote/double-quote' key"
  keymap[VK_OEM_8]                           = KEY_NONE;              // 0xDF Used for miscellaneous characters; it can vary by keyboard.
  keymap[0xE0]                               = KEY_NONE;              // 0xE0 Reserved
  keymap[VK_OEM_AX]                          = KEY_NONE;              // 0xE1 OEM specific
  keymap[VK_OEM_102]                         = KEY_NONE;              // 0xE2 Either the angle bracket key or the backslash key on the RT 102-key keyboard
  keymap[VK_ICO_HELP]                        = KEY_NONE;              // 0xE3 Help key on ICO
  keymap[VK_ICO_00]                          = KEY_NONE;              // 0xE4 00 key on ICO
  keymap[VK_PROCESSKEY]                      = KEY_NONE;              // 0xE5 IME PROCESS key
  keymap[VK_ICO_CLEAR]                       = KEY_NONE;              // 0xE6 OEM specific
  keymap[VK_PACKET]                          = KEY_NONE;              // 0xE7 Used to pass Unicode characters as if they were keystrokes. The VK_PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods. For more information, see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP
  keymap[0xE8]                               = KEY_NONE;              // 0xE8 Unassigned
  keymap[VK_OEM_RESET]                       = KEY_NONE;              // 0xE9 OEM specific,Nokia/Ericsson definitions
  keymap[VK_OEM_JUMP]                        = KEY_NONE;              // 0xEA OEM specific,Nokia/Ericsson definitions
  keymap[VK_OEM_PA1]                         = KEY_NONE;              // 0xEB OEM specific,Nokia/Ericsson definitions
  keymap[VK_OEM_PA2]                         = KEY_NONE;              // 0xEC OEM specific,Nokia/Ericsson definitions
  keymap[VK_OEM_PA3]                         = KEY_NONE;              // 0xED OEM specific,Nokia/Ericsson definitions
  keymap[VK_OEM_WSCTRL]                      = KEY_NONE;              // 0xEE OEM specific,Nokia/Ericsson definitions
  keymap[VK_OEM_CUSEL]                       = KEY_NONE;              // 0xEF OEM specific,Nokia/Ericsson definitions
  keymap[VK_OEM_ATTN]                        = KEY_NONE;              // 0xF0 OEM specific,Nokia/Ericsson definitions
  keymap[VK_OEM_FINISH]                      = KEY_NONE;              // 0xF1 OEM specific,Nokia/Ericsson definitions
  keymap[VK_OEM_COPY]                        = KEY_NONE;              // 0xF2 OEM specific,Nokia/Ericsson definitions
  keymap[VK_OEM_AUTO]                        = KEY_NONE;              // 0xF3 OEM specific,Nokia/Ericsson definitions
  keymap[VK_OEM_ENLW]                        = KEY_NONE;              // 0xF4 OEM specific,Nokia/Ericsson definitions
  keymap[VK_OEM_BACKTAB]                     = KEY_NONE;              // 0xF5 OEM specific,Nokia/Ericsson definitions
  keymap[VK_ATTN]                            = KEY_NONE;              // 0xF6 Attn key
  keymap[VK_CRSEL]                           = KEY_NONE;              // 0xF7 CrSel key
  keymap[VK_EXSEL]                           = KEY_NONE;              // 0xF8 ExSel key
  keymap[VK_EREOF]                           = KEY_NONE;              // 0xF9 Erase EOF key
  keymap[VK_PLAY]                            = KEY_NONE;              // 0xFA Play key
  keymap[VK_ZOOM]                            = KEY_NONE;              // 0xFB Zoom key
  keymap[VK_NONAME]                          = KEY_NONE;              // 0xFC Reserved
  keymap[VK_PA1]                             = KEY_NONE;              // 0xFD PA1 key
  keymap[VK_OEM_CLEAR]                       = KEY_NONE;              // 0xFE Clear key
}
// clang-format on

uint8_t GetModifierKey(uint64_t vk) {
  switch (vk) {
    case VK_SHIFT:
      return KEY_MOD_LSHIFT;
    case VK_CONTROL:
      return KEY_MOD_LCTRL;
    case VK_MENU:
      return KEY_MOD_LALT;
    case VK_LWIN:
      return KEY_MOD_LMETA;
    case VK_RWIN:
      return KEY_MOD_RMETA;
  }
  return KEY_NONE;
}
}  // namespace

void CgvhidClient::Init(int display_width, int display_height) noexcept {
  std::call_once(keymap_initialized, []() { KeyboardMappingInit(); });

  if (!cgvhid_.Init()) {
    ATLTRACE2(atlTraceException, 0, "!Init()\n");
  }
}

int CgvhidClient::KeyboardReset() noexcept {
  keyboard_state_ = {};
  uint8_t key_codes[KEYBD_MAX_KEY_COUNT] = {};
  return cgvhid_.KeyboardUpdate(0, key_codes);
}

int CgvhidClient::KeyboardPress(uint64_t vk) noexcept {
  uint8_t modifier_key = GetModifierKey(vk);
  if (KEY_NONE != modifier_key) {
    return KeyboardModifierPress(modifier_key);
  }

  uint8_t key = keymap[vk];
  if (KEY_NONE == key) {
    return -1;
  }

  if (nullptr == memchr(keyboard_state_.key_codes, key, KEYBD_MAX_KEY_COUNT)) {
    for (int i = 0; i < KEYBD_MAX_KEY_COUNT; ++i) {
      if (KEY_NONE == keyboard_state_.key_codes[i]) {
        keyboard_state_.key_codes[i] = key;
        return cgvhid_.KeyboardUpdate(keyboard_state_.modifiers,
                                      keyboard_state_.key_codes);
      }
    }
    // full
    return -1;
  }

  return cgvhid_.KeyboardUpdate(keyboard_state_.modifiers,
                                keyboard_state_.key_codes);
}

int CgvhidClient::KeyboardRelease(uint64_t vk) noexcept {
  uint8_t modifier_key = GetModifierKey(vk);
  if (KEY_NONE != modifier_key) {
    return KeyboardModifierRelease(modifier_key);
  }

  uint8_t key = keymap[vk];
  if (KEY_NONE == key) {
    return -1;
  }

  auto pos = static_cast<char*>(
      memchr(keyboard_state_.key_codes, key, KEYBD_MAX_KEY_COUNT));
  if (nullptr == pos) {
    return -1;
  }

  *pos = KEY_NONE;
  return cgvhid_.KeyboardUpdate(keyboard_state_.modifiers,
                                keyboard_state_.key_codes);
}

int CgvhidClient::KeyboardModifierPress(uint8_t key) noexcept {
  keyboard_state_.modifiers |= key;
  return cgvhid_.KeyboardUpdate(keyboard_state_.modifiers,
                                keyboard_state_.key_codes);
}

int CgvhidClient::KeyboardModifierRelease(uint8_t key) noexcept {
  keyboard_state_.modifiers &= ~key;
  return cgvhid_.KeyboardUpdate(keyboard_state_.modifiers,
                                keyboard_state_.key_codes);
}

