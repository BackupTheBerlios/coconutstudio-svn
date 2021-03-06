
#include <stdio.h>

namespace GUI {

	void __print_error(const char* p_file,int p_line,const char *p_msg) {
	
		fprintf(stderr,"GUI ERROR: %s:%i - %s\n",p_file,p_line,p_msg);
	}
}

#include "drivers/audio_driver_jack.h"
#include "engine/audio_driver_manager.h"
#include "drivers/file_system_windows.h"
#include "drivers/file_system_dirent.h"
#include "drivers/timer_sdl.h"
#include "drivers/keycodes_sdl.h"
#include "drivers/painter_sdl.h"
#include "widgets/scroll_bar.h"
#include "gui_app/main_window.h"
#include "gui_common/common_skin.h"
#include <SDL.h>

#define DEFAULT_H 550
#define DEFAULT_W 750
#define interface interface

int main_loop(SDL_Surface *screen,TimerSDL *timer,unsigned int flags,MainWindow *main_window) {
	KeycodesSDL keycode_sdl_translator;
	
	bool done=false;
	
	SDL_Event event;
	Uint32 last_click_tick=0;
	bool can_dblclick=false;
	while((!done) && (SDL_WaitEvent(&event))) {
		
		
		switch(event.type) {
			//case SDL_USEREVENT:
		//		HandleUserEvents(&event);
	//			break;
			case SDL_VIDEORESIZE:
				printf("??\n");	
	
				screen = SDL_SetVideoMode  (event.resize.w,event.resize.h, 32, flags);
				main_window->set_size( Size(event.resize.w,event.resize.h ) );
				
				main_window->redraw_all();
				main_window->update();
				
				break;
			case SDL_KEYUP:
			case SDL_KEYDOWN: {
                // Handle any key presses here.
		
				unsigned int mod=0;
				
				if (  event.key.keysym.mod & (KMOD_LSHIFT|KMOD_RSHIFT))
					mod|=KEY_MASK_SHIFT;
				
				if (  event.key.keysym.mod & (KMOD_LALT|KMOD_RALT))
					mod|=KEY_MASK_ALT;
				
				if (  event.key.keysym.mod & (KMOD_LCTRL|KMOD_RCTRL))
					mod|=KEY_MASK_CTRL;
								  

				if (  event.key.keysym.mod & (KMOD_LMETA|KMOD_RMETA)) {
#ifdef META_AS_ALT					
					mod|=KEY_MASK_ALT;
#else
					
					mod|=KEY_MASK_META;
#endif
				}
			
				main_window->key( event.key.keysym.unicode, keycode_sdl_translator.get_code(event.key.keysym.sym), event.key.state==SDL_PRESSED,false, mod );
				
				
				if (event.key.keysym.sym==SDLK_F1) {

					main_window->get_painter()->draw_fill_rect( Point() , Size( screen->w , screen->h ), Color(100,20,33) );
					SDL_UpdateRect(screen, 0,0,0,0);
				}

				//if (event.key.keysym.sym==SDLK_F2) {

				//painter->update_screen();
				//}

				if ((event.key.keysym.mod&KMOD_LSHIFT) && event.key.keysym.sym==SDLK_F12) {
					
					printf("DeadLocking Audio Thread on Purpose! (shift-f12)!!\n");
					SDL_LockAudio();
				}
				
//				printf("Pressed Key %s, unicode %i\n",Keyboard::get_code_name( keycode_sdl_translator.get_code(event.key.keysym.sym)).ascii().get_data(),event.key.keysym.unicode);
			} break;

			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEBUTTONDOWN: {
				
				
				do {
					
					Uint32 last_click_delta=0;
					
					
					if (event.button.type==SDL_MOUSEBUTTONDOWN && event.button.button==BUTTON_LEFT) {
						
						last_click_delta=SDL_GetTicks()-last_click_tick;
						last_click_tick=SDL_GetTicks();
					}
					
																
					SDLMod	mod_state=SDL_GetModState();
	
					unsigned int mod=0;
					
					if (  mod_state & (KMOD_LSHIFT|KMOD_RSHIFT))
						mod|=KEY_MASK_SHIFT;
					
					if (  mod_state & (KMOD_LALT|KMOD_RALT))
						mod|=KEY_MASK_ALT;
					
					if (  mod_state & (KMOD_LCTRL|KMOD_RCTRL))
						mod|=KEY_MASK_CTRL;
									
					if (  mod_state & (KMOD_LMETA|KMOD_RMETA))
						mod|=KEY_MASK_META;
					
					
					main_window->mouse_button( Point( event.button.x, event.button.y ), event.button.button, event.button.type==SDL_MOUSEBUTTONDOWN, mod );
					
					if (can_dblclick && last_click_delta<250 && event.button.type==SDL_MOUSEBUTTONDOWN && event.button.button==BUTTON_LEFT) {
						main_window->mouse_doubleclick( Point( event.button.x, event.button.y ), mod );
						can_dblclick=false;
	
					} else
						can_dblclick=true;
					
				} while (SDL_PeepEvents(&event,1,SDL_GETEVENT,SDL_EVENTMASK(SDL_MOUSEMOTION))>0);

					
				
				
			} break;
				
			case SDL_MOUSEMOTION: {

				can_dblclick=false; //can't doubleclick! wah wah wah
				/* Motion compensation, in case there are MANY motion events pending */
				SDL_Event compensator;
				
				while (SDL_PeepEvents(&compensator,1,SDL_GETEVENT,SDL_EVENTMASK(SDL_MOUSEMOTION))>0) {
					
					event.motion.xrel+=compensator.motion.xrel;
					event.motion.yrel+=compensator.motion.yrel;
					event.motion.state=compensator.motion.state;
					event.motion.x=compensator.motion.x;
					event.motion.y=compensator.motion.y;
				}

				
				int state=event.motion.state&0xFF;
				SDLMod	mod_state=SDL_GetModState();

				if (  mod_state & (KMOD_LSHIFT|KMOD_RSHIFT))
					state|=KEY_MASK_SHIFT;
				
				if (  mod_state & (KMOD_LALT|KMOD_RALT))
					state|=KEY_MASK_ALT;
				
				if (  mod_state & (KMOD_LCTRL|KMOD_RCTRL))
					state|=KEY_MASK_CTRL;
								
				if (  mod_state & (KMOD_LMETA|KMOD_RMETA))
					state|=KEY_MASK_META;

				
				main_window->mouse_motion( Point( event.motion.x, event.motion.y ), Point( event.motion.xrel, event.motion.yrel ), state );
			} break;
				
			case SDL_USEREVENT: {
				
				if (event.user.code!=TimerSDL::SDL_TIMER_EVENT_CODE)
					break;
				
				TimerID *timer_id=(TimerID*)event.user.data1;
				timer->call(*timer_id);
			} break; 
			case SDL_QUIT:
				main_window->quit_request();
				break;

			default: {}
				break;

		}   // End switch
            

//		printf("check updates! - %i\n",i++);
		timer->loop_iterate();
		main_window->check_for_updates();
		//SDL_UpdateRect   (screen, 0, 0, 0, 0);
		
		done=main_window->must_quit();
	}   // End while
        	
	return 0;
	
}

int main(int argc, char *argv[]) {

#ifdef WINDOWS_ENABLED	
	FileSystemWindows::set_default_filesystem();
#else
	FileSystemDirent::set_default_filesystem();

#endif
	
	
#ifdef JACK_ENABLED
	
	AudioDriverJACK driver_jack;
	AudioDriverManager::add_driver( &driver_jack );		
	
#endif
	
	/** ENABLE GUI AND SDL */
	
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}
 
	SDL_EnableUNICODE(1);

	const SDL_VideoInfo *vinfo=SDL_GetVideoInfo();
	Uint32 flags =SDL_SWSURFACE | SDL_RESIZABLE;
	SDL_Surface *screen = SDL_SetVideoMode  (DEFAULT_W, DEFAULT_H,vinfo->vfmt->BitsPerPixel , flags);
	
	PainterSDL *painter = new PainterSDL(screen);
	TimerSDL *timer = new TimerSDL;
	
	ScrollBar::set_can_focus_by_default( false );
	
	PixmapData::set_painter(painter);
	CommonSkin skin(painter);	
	
	MainWindow * main_window = new MainWindow( painter, timer, &skin );
	
	
	main_window->set_size( Size( DEFAULT_W,DEFAULT_H ) );
	main_window->update();
	main_window->redraw_all();
	painter->update_screen();
			
	/**** GUI AND SDL INIT END */
		
	/* config */
	
	main_window->init(""); // no config
	
	int res = main_loop(screen,timer,flags,main_window);
	
	AudioDriver *current_driver = AudioDriverManager::get_driver( AudioDriverManager::get_current() );
	
	if (current_driver && current_driver->is_active())
		current_driver->finish();
	
	return res;

}

