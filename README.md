# **=\~=\~=\~=\~=[ ElysiumHaxx for CS:GO ]=\~=\~=\~=\~=**

>   ##### Based on @HackermanX 's SDK

### Instructions

>   -   Get yourself an injector that supports `manual mapping` (This is very important!)
>   -   Add the `ElysiumHaxx.dll` to your injector’s inputs
>   -   Change the injection mode to `manual map`, and the thread creation mode to `NtCreateThreadEx` (You can probably experiment with the thread creation mode, this is just what I’ve found works best for me, but the manual map is important!)
>   -   Once the game has fully loaded into the main menu, press the inject button on your injector, now wait for a console to appear and this will tell you when the injection is finished.
>   -   Once you have loaded into the first round, press F4 to open the menu.
>   -   Profit ???

### Features:

|       Feature       |                 Status                 |                         Description                          |
| :-----------------: | :------------------------------------: | :----------------------------------------------------------: |
|      BackTrack      |           Fully Implemented            | BackTrack intelligently modifies the latency data that your client sends to the server, to allow you to hit a target if you shoot anywhere in the path that they have travelled in the previous ~350ms, for example you could headshot another player after they have returned to cover, and it will still register the hit server-side! This is certainly a very infuriating hack for the other players! **so use wisely** |
| Recoil Compensation |           Fully Implemented            | `NOTE: full anti-recoil is not possible in cs:go` However it can be compensated for pretty well with the methods used here. Recoil is pretty much negligible, but not gone. |
|         ESP         |   Implemented, still expanding upon    | Highly customizable ESP, with boxes, bones, names, ammo, scoped status, health, with customizable colors for each. |
|        B.Hop        |           Fully Implemented            |             Hold down space to use when enabled.             |
|       Aimbot        | Implemented, many improvements to come | As it stands, the aimbot is pretty primitive. It definitely requires some polish before it is used in public games. Will be adding: smoothing, |
|        Chams        |              Implemented               | Will be adding more options to the menu to allow the selection from more materials. |

### Hooks

| Hooked Function       | Purpose                                                      |
| --------------------- | ------------------------------------------------------------ |
| `EndScene();`         | Used for all rendering, to ensure that our graphics are drawn above all others |
| `DrawModelExecute();` | Used to override the rendering of game models, in this instance it is used to modify the materials of the player models to perform our Chams hack |
| `CreateMove()`        | This function is used to process input from the local player. For our purposes it is used for the aimbot, recoil compensation, backtracking, bhop, and prediction. As you can see, this is one of the most important hooks |
| `OverrideView()`      | This hook is used to populate our viewmatrix information. This is very important for us to be able to perform World2Screen calculations. |
| `FrameStageNotify()`  | This hook is used to perform tasks at certain “frame stages” which represent the timeline from sending new information to the server, to beginning to render the new information on our screen. For our purposes this is used for our backtracking function, because it allows us to ensure that tasks are performed *right before* the game is notified that the network has finished updating, and right before the game is notified that a frame is to be rendered. |
| `LockCursor()`        | This hook is a simple one, it is used to free the mouse cursor when we have our menu open. This allows the menu to be controlled by the mouse. This *should* be an optional feature soon, as I’m hoping to implement a keyboard control system for the menu to have less impact on gameplay. |
| `SendDatagram()`      | This hook is executed every time the game attempts to send a packet from our client, to the server. This is helpful because it allows us to modify any data that we like in said packet **before** it has left our client. Currently this is used to modify the latency values in the packets to allow our backtrack cheat to work. |

