

//
// C++ Implementation: rsskin
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "common_skin.h"



#define SET_STYLEBOX( m_which , m_style )\
set_stylebox_name(m_which,#m_which);\
set_stylebox(m_which,m_style);

#define SET_CONSTANT( m_which , m_constant )\
set_constant_name(m_which,#m_which);\
set_constant(m_which,m_constant);

#define SET_FONT( m_which , m_font )\
set_font_name(m_which,#m_which);\
set_font(m_which,m_font);

#define SET_BITMAP( m_which , m_bitmap )\
set_bitmap_name(m_which,#m_which);\
set_bitmap(m_which,m_bitmap);

#define SET_COLOR( m_which , m_color )\
set_color_name(m_which,#m_which);\
set_color(m_which,m_color);
	
	
	
void CommonSkin::set_default_extra() {
		
	/** MAIN WINDOW **/
	SET_BITMAP(BITMAP_CONTROL_FF,PixmapData::get_pixmap(                PIXMAP_CONTROL_FF));
	SET_BITMAP(BITMAP_CONTROL_LOOP,PixmapData::get_pixmap(              	PIXMAP_CONTROL_LOOP));
	SET_BITMAP(BITMAP_CONTROL_PAUSE,PixmapData::get_pixmap(             	PIXMAP_CONTROL_PAUSE));
	SET_BITMAP(BITMAP_CONTROL_PLAY,PixmapData::get_pixmap(              	PIXMAP_CONTROL_PLAY));
	SET_BITMAP(BITMAP_CONTROL_REC,PixmapData::get_pixmap(               	PIXMAP_CONTROL_REC));
	SET_BITMAP(BITMAP_CONTROL_REC_AUTO,PixmapData::get_pixmap(          	PIXMAP_CONTROL_REC_AUTO));
	SET_BITMAP(BITMAP_CONTROL_RW,PixmapData::get_pixmap(                	PIXMAP_CONTROL_RW));
	SET_BITMAP(BITMAP_CONTROL_STOP,PixmapData::get_pixmap(              	PIXMAP_CONTROL_STOP));
	SET_BITMAP(BITMAP_ICON_AUDIO,PixmapData::get_pixmap(                	PIXMAP_ICON_AUDIO));
	SET_BITMAP(BITMAP_ICON_CURSOR_STEP,PixmapData::get_pixmap(          	PIXMAP_ICON_CURSOR_STEP));
	SET_BITMAP(BITMAP_ICON_DELETE,PixmapData::get_pixmap(               	PIXMAP_ICON_DELETE));
	SET_BITMAP(BITMAP_ICON_EDIT,PixmapData::get_pixmap(                 	PIXMAP_ICON_EDIT));
	SET_BITMAP(BITMAP_ICON_INFO,PixmapData::get_pixmap(                 	PIXMAP_ICON_INFO));
	SET_BITMAP(BITMAP_ICON_PLAY_CURRENT_BLOCK,PixmapData::get_pixmap(PIXMAP_ICON_PLAY_CURRENT_BLOCK));
	SET_BITMAP(BITMAP_ICON_PLAY_FROM_CURSOR,PixmapData::get_pixmap(PIXMAP_ICON_PLAY_FROM_CURSOR));
	SET_BITMAP(BITMAP_ICON_REDO,PixmapData::get_pixmap(PIXMAP_ICON_REDO));
	SET_BITMAP(BITMAP_ICON_SWING,PixmapData::get_pixmap(PIXMAP_ICON_SWING));
	SET_BITMAP(BITMAP_ICON_TEMPO,PixmapData::get_pixmap(PIXMAP_ICON_TEMPO));
	SET_BITMAP(BITMAP_ICON_TIME,PixmapData::get_pixmap(PIXMAP_ICON_TIME));
	SET_BITMAP(BITMAP_ICON_UNDO,PixmapData::get_pixmap(PIXMAP_ICON_UNDO));
	SET_BITMAP(BITMAP_ICON_MUTE,PixmapData::get_pixmap(PIXMAP_ICON_MUTE));
 	SET_BITMAP(BITMAP_ICON_SOLO,PixmapData::get_pixmap(PIXMAP_ICON_SOLO));
 	SET_BITMAP(BITMAP_ICON_COLUMN_ADD,PixmapData::get_pixmap(PIXMAP_ICON_COLUMN_ADD));
 	SET_BITMAP(BITMAP_ICON_COLUMN_REMOVE,PixmapData::get_pixmap(PIXMAP_ICON_COLUMN_REMOVE));
 	SET_BITMAP(BITMAP_ICON_AUTOMATION,PixmapData::get_pixmap(PIXMAP_ICON_AUTOMATION));
 	SET_BITMAP(BITMAP_ICON_LEFT,PixmapData::get_pixmap(PIXMAP_ICON_LEFT));
 	SET_BITMAP(BITMAP_ICON_RIGHT,PixmapData::get_pixmap(PIXMAP_ICON_RIGHT));
 	SET_BITMAP(BITMAP_ICON_RENAME,PixmapData::get_pixmap(PIXMAP_ICON_RENAME));
 	SET_BITMAP(BITMAP_ICON_REMOVE,PixmapData::get_pixmap(PIXMAP_ICON_ERASE));
 	SET_BITMAP(BITMAP_ICON_FILE_DISK,PixmapData::get_pixmap(PIXMAP_ICON_FILE_DISK));
 	SET_BITMAP(BITMAP_ICON_FILE_FOLDER,PixmapData::get_pixmap(PIXMAP_ICON_FILE_FOLDER));
 	SET_BITMAP(BITMAP_ICON_ADD,PixmapData::get_pixmap(PIXMAP_ICON_ADD));
 	SET_BITMAP(BITMAP_ICON_ACTIVE,PixmapData::get_pixmap(PIXMAP_ICON_ACTIVE));
 	SET_BITMAP(BITMAP_ICON_INACTIVE,PixmapData::get_pixmap(PIXMAP_ICON_INACTIVE));

	/* GLOBAL VIEW */
	
	SET_FONT(FONT_GLOBAL_VIEW,0);
	SET_FONT(FONT_GLOBAL_VIEW_BAR,bar_font);
 	SET_FONT(FONT_GLOBAL_VIEW_BEAT,beat_font);
	SET_FONT(FONT_GLOBAL_VIEW_MARKER,0);

	
	

	SET_COLOR(COLOR_GLOBAL_VIEW_FONT,Color(211,211,211));
	SET_COLOR(COLOR_GLOBAL_VIEW_FONT_SHADOW,Color(31,31,31));
	SET_COLOR(COLOR_GLOBAL_VIEW_BAR_FONT,Color(200,150,150));
	SET_COLOR(COLOR_GLOBAL_VIEW_BEAT_FONT,Color(150,150,200));
	SET_COLOR(COLOR_GLOBAL_VIEW_MARKER_FONT,Color(220,220,220));
	SET_COLOR(COLOR_GLOBAL_VIEW_BAR_LINE,Color(80,120,140));
	SET_COLOR(COLOR_GLOBAL_VIEW_BEAT_LINE,Color(50,60,80));
	SET_COLOR(COLOR_GLOBAL_VIEW_SEPARATOR,Color(90,90,90));
	SET_COLOR(COLOR_GLOBAL_VIEW_AUTOMATION,Color(0));
	SET_COLOR(COLOR_GLOBAL_VIEW_PATTERN_NOTE,Color(0));
	SET_COLOR(COLOR_GLOBAL_VIEW_LOOP,Color(120,120,120));
	SET_COLOR(COLOR_GLOBAL_VIEW_LOOP_MARKER,Color(250,250,250));
	SET_COLOR(COLOR_GLOBAL_VIEW_BG,Color(0));
	SET_COLOR(COLOR_GLOBAL_VIEW_MOUSE_OVER_BEAT,Color(20,180,100));
	SET_COLOR(COLOR_GLOBAL_VIEW_BARBEAT_CHANGE,Color(180,120,100));
	SET_STYLEBOX(SB_GLOBAL_VIEW_PATTERN,StyleBox(2,Color(0x81,0xAB,0xB3),Color(0xEE,0xFF,0xEE),Color(0xDE,0xEF,0xDE)));
	SET_STYLEBOX(SB_GLOBAL_VIEW_PATTERN_SELECTED,StyleBox(2,Color(0xB3,0x88,0x81),Color(0xFF,0xEE,0xEE),Color(0xEF,0xDE,0xDE)));
	SET_STYLEBOX(SB_GLOBAL_VIEW_AUTOMATION,StyleBox(2,Color(0xAC,0xB3,0x81),Color(0xEE,0xFF,0xFF),Color(0xDE,0xEE,0xEE)));
	SET_STYLEBOX(SB_GLOBAL_VIEW_AUTOMATION_SELECTED,StyleBox(2,Color(0xB3,0x88,0x81),Color(0xFF,0xEE,0xEE),Color(0xEF,0xDE,0xDE)));
	SET_STYLEBOX(SB_GLOBAL_VIEW_REPEAT,StyleBox(2,Color(0xCC),Color(0xFF),Color(0xEE)));
	
	{
		
		StyleBox forbid_sb;
	
		forbid_sb.mode=StyleBox::MODE_BITMAP;
		
		forbid_sb.bitmap=PixmapData::get_pixmap( PIXMAP_STYLE_FORBID_CENTER );
		
		SET_STYLEBOX(SB_GLOBAL_VIEW_FORBIDDEN,forbid_sb);
	}
	
	{
		
		StyleBox linked_sb;
	
		/* linked_sb.mode=StyleBox::MODE_FLAT_BITMAP;
		
		linked_sb.bitmaps[StyleBox::POS_TOPLEFT]=PixmapData::get_pixmap( PIXMAP_STYLE_LINKED_CHAIN_CORNER);
		linked_sb.bitmaps[StyleBox::POS_TOPRIGHT]=PixmapData::get_pixmap( PIXMAP_STYLE_LINKED_CHAIN_CORNER);
		linked_sb.bitmaps[StyleBox::POS_BOTTOMLEFT]=PixmapData::get_pixmap( PIXMAP_STYLE_LINKED_CHAIN_CORNER);
		linked_sb.bitmaps[StyleBox::POS_BOTTOMRIGHT]=PixmapData::get_pixmap( PIXMAP_STYLE_LINKED_CHAIN_CORNER);
		
		linked_sb.bitmaps[StyleBox::POS_LEFT]=PixmapData::get_pixmap( PIXMAP_STYLE_LINKED_CHAIN);
		linked_sb.bitmaps[StyleBox::POS_RIGHT]=PixmapData::get_pixmap( PIXMAP_STYLE_LINKED_CHAIN);
		
		linked_sb.bitmaps[StyleBox::POS_TOP]=PixmapData::get_pixmap( PIXMAP_STYLE_LINKED_CHAIN_CORNER);
		linked_sb.bitmaps[StyleBox::POS_BOTTOM]=PixmapData::get_pixmap( PIXMAP_STYLE_LINKED_CHAIN_CORNER);
		
		linked_sb.draw_center=false; */
		
		SET_STYLEBOX(SB_GLOBAL_VIEW_LINKED,linked_sb);
	}
	
	
	
	/* Global View ToolBar */
	
	

	SET_BITMAP(BITMAP_GLOBAL_TOOLBAR__SELECT,PixmapData::get_pixmap(PIXMAP_ICON_SELECT));
	SET_BITMAP(BITMAP_GLOBAL_TOOLBAR__ADD,PixmapData::get_pixmap(PIXMAP_ICON_EDIT));
	SET_BITMAP(BITMAP_GLOBAL_TOOLBAR__COPY,PixmapData::get_pixmap(PIXMAP_ICON_COPY));
	SET_BITMAP(BITMAP_GLOBAL_TOOLBAR__COPY_LINK,PixmapData::get_pixmap(PIXMAP_ICON_COPY_LINK));
	SET_BITMAP(BITMAP_GLOBAL_TOOLBAR__DELETE_SELECTED,PixmapData::get_pixmap(PIXMAP_ICON_DELETE));
	SET_BITMAP(BITMAP_GLOBAL_TOOLBAR__SELECT_LINKED,PixmapData::get_pixmap(PIXMAP_ICON_SHOW_LINKS));
	SET_BITMAP(BITMAP_GLOBAL_TOOLBAR__UNLINK_SELECTED,PixmapData::get_pixmap(PIXMAP_ICON_UNLINK));
	SET_BITMAP(BITMAP_GLOBAL_TOOLBAR__ZOOM,PixmapData::get_pixmap(PIXMAP_ICON_ZOOM));
		
	SET_CONSTANT(C_GLOBAL_VIEW_LOOP_COLUMN_WIDTH,16);
	SET_CONSTANT(C_GLOBAL_VIEW_MARKER_COLUMN_MARGIN,2);
	
	
	/* EDIT VIEW */
	
	
	StyleBox cursor_sb = StyleBox(1,Color(255),Color(255),Color(255));
	cursor_sb.draw_center=false;

	SET_STYLEBOX(SB_EDIT_VIEW_CURSOR,cursor_sb);
	SET_CONSTANT(C_EDITOR_ROW_MARGIN,3);
	SET_CONSTANT(C_EDIT_VIEW_SIDE_WIDGET_MARGIN,1);
	SET_COLOR(COLOR_EDITOR_BG,Color(0));
	SET_COLOR(COLOR_EDIT_VIEW_SIDE_WIDGET_AUTOMATION,Color(0xff,0xff,0xc8));
       	SET_COLOR(COLOR_EDIT_VIEW_SIDE_WIDGET_PATTERN,Color(0xae,0xe9,0xf8));
	SET_BITMAP(BITMAP_EDIT_VIEW_SIDE_WIDGET_ARROW_PATTERN, PixmapData::get_pixmap( PIXMAP_SIDE_WIDGET_PATTERN ) );
	SET_BITMAP(BITMAP_EDIT_VIEW_SIDE_WIDGET_ARROW_AUTOMATION, PixmapData::get_pixmap( PIXMAP_SIDE_WIDGET_AUTOMATION ) );
	SET_FONT(FONT_EDIT_VIEW_SIDE_WIDGET,0);
	SET_FONT(FONT_EDIT_VIEW_MARKER_FONT,0);
	
	SET_COLOR(COLOR_EDIT_VIEW_ROW_BAR_FONT,Color(0xe2,0xf6,0xff));
	SET_COLOR(COLOR_EDIT_VIEW_ROW_BEAT_FONT,Color(0x7a,0x90,0x9c));
	SET_COLOR(COLOR_EDIT_VIEW_ROW_SUBBEAT_FONT,Color(0x4f,0x5b,0x65));
	SET_COLOR(COLOR_EDIT_VIEW_MARKER_FONT,Color(220,220,220));
	
	/* AUTOMATION */
	
	SET_FONT(FONT_AUTOMATION_VALUE,0);
	SET_COLOR(COLOR_AUTOMATION_FONT_SHADOW,Color(0));
     	SET_COLOR(COLOR_AUTOMATION_FONT,Color(255));
	
      	SET_COLOR(COLOR_AUTOMATION_BEAT_LINE,Color(0x70,0x7a,0x63));
      	SET_COLOR(COLOR_AUTOMATION_BAR_LINE,Color(0xcd,0xb7,0xbc));
      	SET_COLOR(COLOR_AUTOMATION_SUBBEAT_LINE,Color(0x41,0x47,0x2e));

      	SET_COLOR(COLOR_AUTOMATION_ENVELOPE,Color(0xff,0xff,0xc8));
      	SET_COLOR(COLOR_AUTOMATION_ENVELOPE_REPEAT,Color(0xa0,0xa0,0x50));
	
	SET_COLOR(COLOR_AUTOMATION_BG,Color(0));
	SET_COLOR(COLOR_AUTOMATION_SELECTION,Color(0x75,0x7f,0x9));
	
	SET_CONSTANT(C_AUTOMATION_MARGIN_WIDTH,5);
   	SET_CONSTANT(C_AUTOMATION_SIZE_SMALL,60);
   	SET_CONSTANT(C_AUTOMATION_SIZE_MEDIUM,120);
   	SET_CONSTANT(C_AUTOMATION_SIZE_BIG,180);
   	SET_CONSTANT(C_AUTOMATION_SELECTION_MARGIN,3);
	
	SET_BITMAP(BITMAP_AUTOMATION_POINT,PixmapData::get_pixmap(PIXMAP_ENVELOPE_POINT));
	
	SET_STYLEBOX(SB_AUTOMATION,StyleBox(3,Color(0x2f,0x37,0x1e),Color(0xcd,0xd7,0xbc),Color(0xcd,0xd7,0xbc)));
     	SET_STYLEBOX(SB_AUTOMATION_SHARED,get_stylebox(SB_GLOBAL_VIEW_LINKED)); //use the same

	/* PATTERN */
	
	SET_FONT(FONT_PATTERN,0);
	SET_COLOR(COLOR_PATTERN_FONT,Color(0xae,0xe9,0xf8));
       	SET_COLOR(COLOR_PATTERN_FONT_VOLUME,Color(0xf2,0xae,0xbd));
       	SET_COLOR(COLOR_PATTERN_FONT_NOFIT,Color(0x81,0x81,0x81));
       	SET_COLOR(COLOR_PATTERN_FONT_REPEAT,Color(0x81,0x81,0x81));
	SET_COLOR(COLOR_PATTERN_VOLUME_BG,Color(0));
	SET_COLOR(COLOR_PATTERN_NOTE_BG,Color(0));
	SET_COLOR(COLOR_PATTERN_VOLUME_BAR,Color(0xff,0xbe,0xaa));
	SET_COLOR(COLOR_PATTERN_NOTE_BAR,Color(0xc8,0xdc,0xff));
	SET_COLOR(COLOR_PATTERN_BEAT_LINE,Color(0x7a,0x90,0x9c));
     	SET_COLOR(COLOR_PATTERN_BAR_LINE,Color(0xe2,0xf6,0xff));
      	SET_COLOR(COLOR_PATTERN_SUBBEAT_LINE,Color(0x4f,0x5b,0x65));
	SET_COLOR(COLOR_PATTERN_SELECTION,Color(0x28,0x5C,0x7C));
	SET_COLOR(COLOR_PATTERN_BG,Color(0));
	
	SET_CONSTANT(C_PATTERN_VOL_NOTE_BAR_HEIGHT,2);
	SET_CONSTANT(C_PATTERN_SELECTION_MARGIN,3);

	SET_STYLEBOX(SB_PATTERN,StyleBox(3,Color(0x2d,0x27,0x39),Color(0xbd,0xd3,0xd7),Color(0xbd,0xd3,0xd7)));
     	SET_STYLEBOX(SB_PATTERN_SHARED,get_stylebox(SB_GLOBAL_VIEW_LINKED)); //use the same

	/* PLUGIN LIST */
	
  	SET_COLOR( COLOR_PLUGIN_CHOOSER_FONT_NAME, Color(0) );
  	SET_COLOR( COLOR_PLUGIN_CHOOSER_FONT_DESCRIPTION, Color(120) );
  	SET_COLOR( COLOR_PLUGIN_CHOOSER_BG, Color(255) );
  	SET_COLOR( COLOR_PLUGIN_CHOOSER_BG_SELECTED, Color( 180,190,255 ));
  	SET_COLOR( COLOR_PLUGIN_CHOOSER_SEPARATOR, Color(180) );
  	SET_COLOR( COLOR_PLUGIN_CHOOSER_SECTION, Color(0) );
	
	SET_FONT( FONT_PLUGIN_CHOOSER_SECTION, 0 );
	SET_FONT( FONT_PLUGIN_CHOOSER_NAME, 0 );
	SET_FONT( FONT_PLUGIN_CHOOSER_DESCRIPTION, 0 );
	
	SET_CONSTANT( C_PLUGIN_CHOOSER_ITEM_MARGIN, 3 );
	SET_CONSTANT( C_PLUGIN_CHOOSER_ITEM_INTERNAL_SEPARATION, 2 );
	SET_CONSTANT( C_PLUGIN_CHOOSER_ITEM_DESCRIPTION_MARGIN,10);
      	
      	
      	/* AUDIO GRAPH */
 	SET_COLOR( COLOR_GRAPH_AUDIO_CABLE,Color(0xF1,0x6c,0x6c) );
	SET_COLOR( COLOR_GRAPH_EVENT_CABLE,Color(0x6c,0x6c,0xF1) );
	SET_COLOR( COLOR_GRAPH_CONTROL_CABLE,Color(0xf1,0xef,0x6c) );
	SET_COLOR( COLOR_GRAPH_CONNECTING_CABLE, Color(250) );

	
	StyleBox sb;
	sb = StyleBox( 1, Color(0x99,0x5c,0x5c),Color(0xF1,0x6c,0x6c),Color(0xF1,0x6c,0x6c)); 
	sb.margins[0]=2; sb.margins[1]=2; sb.margins[2]=2; sb.margins[3]=2;
     	SET_STYLEBOX( SB_GRAPH_AUDIO_PORT, sb );
	sb = StyleBox( 1, Color(0x5c,0x5c,0x99), Color(0x6c,0x6c,0xF1), Color(0x6c,0x6c,0xF1) );
	sb.margins[0]=2; sb.margins[1]=2; sb.margins[2]=2; sb.margins[3]=2;
     	SET_STYLEBOX( SB_GRAPH_EVENT_PORT, sb);
	sb = StyleBox( 1, Color(0x99,0x98,0x5c),Color(0xf1,0xef,0x6c),Color(0xf1,0xef,0x6c)); 
	sb.margins[0]=2; sb.margins[1]=2; sb.margins[2]=2; sb.margins[3]=2;
     	SET_STYLEBOX( SB_GRAPH_CONTROL_PORT, StyleBox( 1, Color(0x99,0x98,0x5c),Color(0xf1,0xef,0x6c),Color(0xf1,0xef,0x6c)) );

	static const Color layer_colors[MAX_LAYER_COLORS+1]={ 
		Color(0xff,0x74,0x74), Color(0xcc,0xff,0xe5 ),Color(0xc2,0x91,0xff ), Color(0x72,0xa4,0xff),
		Color(0x87,0xea,0xff), Color( 0x45,0xd7,0x4c),Color(0xcc,0xf6,0x5d), Color(0xf6,0xe3,0x41),
		Color(0xcb,0x67,0x6f), Color(0xff,0x91,0xc6),Color(0xe8,0xd0,0x90), Color(0xff,0x9a,0x23),Color(0xA0,0xA0,0xA0)
	};
	
	
	Color dark_layer_colors[MAX_LAYER_COLORS+1];
	Color grayed_layer_colors[MAX_LAYER_COLORS+1];
	
	
	for (int i=0;i<MAX_LAYER_COLORS+1;i++) {
	
		Color c = layer_colors[i];
  		dark_layer_colors[i]=Color( c.r / 4 , c.g / 4, c.b / 4 );
  		int gray = ((int)c.r+(int)c.g+(int)c.b) / 3;
  		grayed_layer_colors[i] = Color( (c.r+gray)/4, (c.g+gray)/4, (c.b+gray)/4 );  		  		
	}
	
	SET_COLOR( COLOR_LAYER_0, layer_colors[0] );
	SET_COLOR( COLOR_LAYER_1, layer_colors[1] );
	SET_COLOR( COLOR_LAYER_2, layer_colors[2] );
	SET_COLOR( COLOR_LAYER_3, layer_colors[3] );
	SET_COLOR( COLOR_LAYER_4, layer_colors[4] );
	SET_COLOR( COLOR_LAYER_5, layer_colors[5] );
	SET_COLOR( COLOR_LAYER_6, layer_colors[6] );
	SET_COLOR( COLOR_LAYER_7, layer_colors[7] );
	SET_COLOR( COLOR_LAYER_8, layer_colors[8] );
	SET_COLOR( COLOR_LAYER_9, layer_colors[9] );
	SET_COLOR( COLOR_LAYER_10, layer_colors[10] );
	SET_COLOR( COLOR_LAYER_11, layer_colors[11] );
	SET_COLOR( COLOR_LAYER_ALL, layer_colors[12] );
	
     	SET_STYLEBOX( SB_LAYER_COLOR_NODE_0, StyleBox(1,grayed_layer_colors[0],layer_colors[0],layer_colors[0]) );
     	SET_STYLEBOX( SB_LAYER_COLOR_NODE_1, StyleBox(1,grayed_layer_colors[1],layer_colors[1],layer_colors[1]) );
     	SET_STYLEBOX( SB_LAYER_COLOR_NODE_2, StyleBox(1,grayed_layer_colors[2],layer_colors[2],layer_colors[2]) );
     	SET_STYLEBOX( SB_LAYER_COLOR_NODE_3, StyleBox(1,grayed_layer_colors[3],layer_colors[3],layer_colors[3]) );
     	SET_STYLEBOX( SB_LAYER_COLOR_NODE_4, StyleBox(1,grayed_layer_colors[4],layer_colors[4],layer_colors[4]) );
     	SET_STYLEBOX( SB_LAYER_COLOR_NODE_5, StyleBox(1,grayed_layer_colors[5],layer_colors[5],layer_colors[5]) );
     	SET_STYLEBOX( SB_LAYER_COLOR_NODE_6, StyleBox(1,grayed_layer_colors[6],layer_colors[6],layer_colors[6]) );
     	SET_STYLEBOX( SB_LAYER_COLOR_NODE_7, StyleBox(1,grayed_layer_colors[7],layer_colors[7],layer_colors[7]) );
     	SET_STYLEBOX( SB_LAYER_COLOR_NODE_8, StyleBox(1,grayed_layer_colors[8],layer_colors[8],layer_colors[8]) );
     	SET_STYLEBOX( SB_LAYER_COLOR_NODE_9, StyleBox(1,grayed_layer_colors[9],layer_colors[9],layer_colors[9]) );
     	SET_STYLEBOX( SB_LAYER_COLOR_NODE_10, StyleBox(1,grayed_layer_colors[10],layer_colors[10],layer_colors[10]) );
     	SET_STYLEBOX( SB_LAYER_COLOR_NODE_11, StyleBox(1,grayed_layer_colors[11],layer_colors[11],layer_colors[11]) );
     	SET_STYLEBOX( SB_LAYER_COLOR_NODE_ALL, StyleBox(1,grayed_layer_colors[12],layer_colors[12],layer_colors[12]) );
     	
     	for (int i=0;i<(MAX_LAYER_COLORS+1);i++) {
     	
     		StyleBox sb = get_stylebox( SB_LAYER_COLOR_NODE_0+i );
     		sb.margins[0]=4; sb.margins[1]=2; sb.margins[2]=4; sb.margins[3]=2;
     		set_stylebox( SB_LAYER_COLOR_NODE_0+i, sb );
     	}
     	
     	SET_STYLEBOX( SB_LAYER_COLOR_BG_0, StyleBox(0,dark_layer_colors[0],Color(0),Color(0)) );
     	SET_STYLEBOX( SB_LAYER_COLOR_BG_1, StyleBox(0,dark_layer_colors[1],Color(0),Color(0)) );
     	SET_STYLEBOX( SB_LAYER_COLOR_BG_2, StyleBox(0,dark_layer_colors[2],Color(0),Color(0)) );
     	SET_STYLEBOX( SB_LAYER_COLOR_BG_3, StyleBox(0,dark_layer_colors[3],Color(0),Color(0)) );
     	SET_STYLEBOX( SB_LAYER_COLOR_BG_4, StyleBox(0,dark_layer_colors[4],Color(0),Color(0)) );
     	SET_STYLEBOX( SB_LAYER_COLOR_BG_5, StyleBox(0,dark_layer_colors[5],Color(0),Color(0)) );
     	SET_STYLEBOX( SB_LAYER_COLOR_BG_6, StyleBox(0,dark_layer_colors[6],Color(0),Color(0)) );
     	SET_STYLEBOX( SB_LAYER_COLOR_BG_7, StyleBox(0,dark_layer_colors[7],Color(0),Color(0)) );
     	SET_STYLEBOX( SB_LAYER_COLOR_BG_8, StyleBox(0,dark_layer_colors[8],Color(0),Color(0)) );
     	SET_STYLEBOX( SB_LAYER_COLOR_BG_9, StyleBox(0,dark_layer_colors[9],Color(0),Color(0)) );
     	SET_STYLEBOX( SB_LAYER_COLOR_BG_10, StyleBox(0,dark_layer_colors[10],Color(0),Color(0)) );
     	SET_STYLEBOX( SB_LAYER_COLOR_BG_11, StyleBox(0,dark_layer_colors[11],Color(0),Color(0)) );
     	SET_STYLEBOX( SB_LAYER_COLOR_BG_ALL, StyleBox(0,dark_layer_colors[12],Color(0),Color(0)) );
     	
     	SET_STYLEBOX( SB_LAYER_COLOR_WINDOW_0, StyleBox(1,grayed_layer_colors[0],layer_colors[0],layer_colors[0]) );
     	SET_STYLEBOX( SB_LAYER_COLOR_WINDOW_1, StyleBox(1,grayed_layer_colors[1],layer_colors[1],layer_colors[1]) );
     	SET_STYLEBOX( SB_LAYER_COLOR_WINDOW_2, StyleBox(1,grayed_layer_colors[2],layer_colors[2],layer_colors[2]) );
     	SET_STYLEBOX( SB_LAYER_COLOR_WINDOW_3, StyleBox(1,grayed_layer_colors[3],layer_colors[3],layer_colors[3]) );
     	SET_STYLEBOX( SB_LAYER_COLOR_WINDOW_4, StyleBox(1,grayed_layer_colors[4],layer_colors[4],layer_colors[4]) );
     	SET_STYLEBOX( SB_LAYER_COLOR_WINDOW_5, StyleBox(1,grayed_layer_colors[5],layer_colors[5],layer_colors[5]) );
     	SET_STYLEBOX( SB_LAYER_COLOR_WINDOW_6, StyleBox(1,grayed_layer_colors[6],layer_colors[6],layer_colors[6]) );
     	SET_STYLEBOX( SB_LAYER_COLOR_WINDOW_7, StyleBox(1,grayed_layer_colors[7],layer_colors[7],layer_colors[7]) );
     	SET_STYLEBOX( SB_LAYER_COLOR_WINDOW_8, StyleBox(1,grayed_layer_colors[8],layer_colors[8],layer_colors[8]) );
     	SET_STYLEBOX( SB_LAYER_COLOR_WINDOW_9, StyleBox(1,grayed_layer_colors[9],layer_colors[9],layer_colors[9]) );
     	SET_STYLEBOX( SB_LAYER_COLOR_WINDOW_10, StyleBox(1,grayed_layer_colors[10],layer_colors[10],layer_colors[10]) );
     	SET_STYLEBOX( SB_LAYER_COLOR_WINDOW_11, StyleBox(1,grayed_layer_colors[11],layer_colors[11],layer_colors[11]) );
     	SET_STYLEBOX( SB_LAYER_COLOR_WINDOW_ALL, StyleBox(1,grayed_layer_colors[11],layer_colors[11],layer_colors[11]) );
	
	SET_FONT( FONT_GRAPH_PORT_PLUG, 0 );
	SET_FONT( FONT_GRAPH_NODE_NAME, 0 );

	SET_BITMAP(BITMAP_GRAPH_PLUG,PixmapData::get_pixmap(PIXMAP_GRAPH_PLUG));
	SET_BITMAP(BITMAP_LAYER_0,PixmapData::get_pixmap(PIXMAP_ICON_LAYER_0));
	SET_BITMAP(BITMAP_LAYER_1,PixmapData::get_pixmap(PIXMAP_ICON_LAYER_1));
	SET_BITMAP(BITMAP_LAYER_2,PixmapData::get_pixmap(PIXMAP_ICON_LAYER_2));
	SET_BITMAP(BITMAP_LAYER_3,PixmapData::get_pixmap(PIXMAP_ICON_LAYER_3));
	SET_BITMAP(BITMAP_LAYER_4,PixmapData::get_pixmap(PIXMAP_ICON_LAYER_4));
	SET_BITMAP(BITMAP_LAYER_5,PixmapData::get_pixmap(PIXMAP_ICON_LAYER_5));
	SET_BITMAP(BITMAP_LAYER_6,PixmapData::get_pixmap(PIXMAP_ICON_LAYER_6));
	SET_BITMAP(BITMAP_LAYER_7,PixmapData::get_pixmap(PIXMAP_ICON_LAYER_7));
	SET_BITMAP(BITMAP_LAYER_8,PixmapData::get_pixmap(PIXMAP_ICON_LAYER_8));
	SET_BITMAP(BITMAP_LAYER_9,PixmapData::get_pixmap(PIXMAP_ICON_LAYER_9));
	SET_BITMAP(BITMAP_LAYER_10,PixmapData::get_pixmap(PIXMAP_ICON_LAYER_10));
	SET_BITMAP(BITMAP_LAYER_11,PixmapData::get_pixmap(PIXMAP_ICON_LAYER_11));
	SET_BITMAP(BITMAP_LAYER_ALL,PixmapData::get_pixmap(PIXMAP_ICON_LAYER_ALL));



	SET_CONSTANT( C_GRAPH_PORT_VSPACING, 2);
	SET_CONSTANT( C_GRAPH_PORT_HSPACING, 2);
	
	SET_COLOR(COLOR_GRAPH_NODE_NAME,Color(255));
	SET_COLOR(COLOR_GRAPH_NODE_PORT,Color(190));
	
	
}
	
	
FontID CommonSkin::load_number_font(PixmapDataList p_pixmap) {
		
	
	int font_h=painter->get_bitmap_size(PixmapData::get_pixmap(p_pixmap)).height-2;
	int font_w=painter->get_bitmap_size(PixmapData::get_pixmap(p_pixmap)).width-11;
	font_w/=10;
	
	FontID font = painter->create_font(font_h,font_h);
	
	for (int i=0;i<10;i++) {
		
		painter->font_add_char(font,'0'+i,PixmapData::get_pixmap(p_pixmap),Rect(Point(1+i*(font_w+1),1),Size(font_w,font_h)));
	}	
	
	return font;
}
	
CommonSkin::CommonSkin(Painter *p_painter) : Skin(COMMON_SB_MAX,COMMON_C_MAX,COMMON_BITMAP_MAX,COMMON_FONT_MAX,COMMON_COLOR_MAX) {
	
	painter=p_painter;
	
	bar_font=load_number_font(PIXMAP_GLOBAL_VIEW_BAR_FONT);
	beat_font=load_number_font(PIXMAP_GLOBAL_VIEW_BEAT_FONT);
	
	set_default_extra();
}


CommonSkin::~CommonSkin()
{
}



