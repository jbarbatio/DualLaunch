# DualLaunch
Sometimes a game is annoying and uses one app to authenticate and get the commandline for the real app. This makes it more tedious to launch the real app and inject a dll. One must first launch the auth app, breakpoint, grab the command line, then pass that command line to whatever program one is using to launch the real app and inject.

DualLaunch will launch the first app, detour ShellExecute, save the command line passed, and exit.
DualLaunch will then launch the second app with the saved command line and inject 'Second.dll'

Of course, ShellExecute is not the only way to luanch a program so some customization may be needed.

Further, there are a million other ways to inject code, this is an example of one.
