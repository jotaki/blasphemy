#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>

int rgb_display(long int rgb)
{
	Display * display;
	Window window, root;
	XEvent event;

	display = XOpenDisplay(NULL);
	if(display == NULL) {
		fprintf(stderr, "Cannot open display.\n");
		return 1;
	}

	root = RootWindow(display, DefaultScreen(display));
	window = XCreateSimpleWindow(display, root, 10, 10, 250, 250,
			CopyFromParent, CopyFromParent, (unsigned) rgb);

	XMapWindow(display, window);
	XSelectInput(display, window, KeyPressMask);

	while(1) {
		XNextEvent(display, &event);

		if(event.type == KeyPress)
			break;
	}

	XCloseDisplay(display);
	return 0;
}

int main(int argc, char *argv[])
{
	if(argc == 1) {
		printf("Usage: %s <rgb color>\n", argv[0]);
		return 1;
	}

	return rgb_display(strtol(argv[1], NULL, 16));
}
