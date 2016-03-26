#include "SUI.h"

widget_table global_widget_table;

ret_code draw_general_button(bound bound, char *text, color c_background, color c_frame, color c_font){
    //Full the Background
	fill_area(bound, c_background);

    //Draw the frame
    draw_Hline(bound.x, bound.y.min, c_frame);
    draw_Hline(bound.x, bound.y.max, c_frame);
    draw_Vline(bound.x.min, bound.y, c_frame);
    draw_Vline(bound.x.max, bound.y, c_frame);

    //Draw the text
    draw_general_textfield(bound, text, 24, V_ALIGN_CENTER|H_ALIGN_CENTER, TRANSPARENT, c_font);
    return OK;
}


// Handle the align
ret_code draw_general_textfield(bound bound, char *text, alt_u16 size, align align, color c_background, color c_font){
    //Full the Background
	fill_area(bound, c_background);

    //Draw the text
    int h_align, v_align;
    point origin;
    origin.x = bound.x.min;
    origin.y = bound.y.min;
    h_align = align & 0x0f;
    v_align = align & 0xf0;
    //printf("%d : %d, %d\n", align, h_align, v_align);
    switch(h_align){
    case H_ALIGN_LEFT:
        origin.x = bound.x.min;
        break;
    case H_ALIGN_CENTER:
        origin.x = bound.x.min + ((bound.x.max-bound.x.min) - size/2*strlen(text)) / 2;
        break;
    case H_ALIGN_RIGHT:
        origin.x = bound.x.max - size/2 * strlen(text);
        break;
    }
    switch(v_align){
    case V_ALIGN_TOP:
        origin.y = bound.y.min;
        break;
    case V_ALIGN_CENTER:
        origin.y = bound.y.min + ((bound.y.max - bound.y.min) - size)/2;
        break;
    case V_ALIGN_BOTTOM:
        origin.y = bound.y.max - size;
        break;
    }
    draw_string(origin, text, size, c_font);
    return OK;
}

// Just a single line, but can parted fresh
ret_code draw_general_textfield_part_refresh(bound bounds, char *last_text, char* text, alt_u16 size,
    color c_background, color c_font){

	int i, min_len, max_len;
	char buffer[2];
	bound area;
    point origin;

	if(strlen(text) > strlen(last_text)){
		min_len = strlen(last_text);
        max_len = strlen(text);
	}else{
		min_len = strlen(text);
        max_len = strlen(last_text);
	}

	for(i = 0; i < min_len; i++){	 //draw to the min length
		if(last_text[i] != text[i]){
			buffer[0] = text[i];
			buffer[1] = '\0';
            area.x.min = bounds.x.min + i * size / 2;
            area.x.max = bounds.x.min + (i + 1) * size / 2;
            area.y = bounds.y;
            origin.x = area.x.min;
            origin.y = area.y.min;
			fill_area(area, c_background);
			draw_string(origin, buffer, size, c_font);
		}
	}

    area.x.min = bounds.x.min + i * size / 2;
    area.x.max = bounds.x.min + max_len * size / 2;
    area.y = bounds.y;
	fill_area(area, c_background); //clear later text

	if(min_len < strlen(text)){  //if the text printed shorter than truly length
        origin.x = area.x.min;
        origin.y = bounds.y.min;
		draw_string(origin, text + i, size, c_font);
	}
	return OK;
}

// mainly handle the the '\n' for LF
ret_code draw_general_multi_textfield(bound bound, char *text, alt_u16 size, color c_background, color c_font){
    int i, str_len;
    char *sub_str;
    sub_str = text;
    str_len = strlen(text);
    for(i = 0; i < str_len + 1; i++){
        if(text[i] == '\n' || text[i] == '\0'){
            text[i] = '\0';
            draw_general_multi_textfield_none_LF(bound, sub_str, size, c_background, c_font);
            //printf("%s\n", sub_str);

            sub_str = &text[i+1];  //refresh sub_str
            bound.y.min += size;   //refresh bound for showing
            if(bound.y.min + size > bound.y.max){ //if too much line
                return ERROR;
            }
        }
    }
    return OK;
}

//Mainly Handle word warp
ret_code draw_general_multi_textfield_none_LF(bound bound, char *text, alt_u16 size, color c_background, color c_font){
	int str_len, max_char;
	char *sub_str;
	str_len = strlen(text);
	max_char = (bound.x.max - bound.x.min) / (size/2); //assume that the width of char is the half size
	sub_str = malloc(sizeof(char) * (max_char + 1));
	do{
		if(str_len >  max_char){
			strncpy(sub_str, text, max_char);
			sub_str[max_char] = '\0';
			text += max_char;
			str_len -= max_char;
		}else{
			strncpy(sub_str, text, str_len + 1);
			str_len = 0;
		}
		draw_general_textfield(bound, sub_str, size, H_ALIGN_LEFT|V_ALIGN_TOP, c_background, c_font);
		bound.y.min += size;
	}while(str_len > 0 && bound.y.min + size < bound.y.max);
	free(sub_str);
    return OK;
}

/*                              *
 *        Function Part         *
 *                              */

inline void hide_widget(int id){
	widget *wid = get_widget_from_id(id);
	if(wid->ishided == TRUE){
		return;
	}

    messages message;
    message.dest = id;
    message.msgid = WIDGET_HIDE;
    message.data = NULL;
    send_message(message);
    wid->ishided = TRUE;
}

inline void show_widget(int id){
	widget *wid = get_widget_from_id(id);
//	if(wid->ishided == FALSE){
//		return;
//	}
    messages message;
    message.dest = id;
    message.msgid = WIDGET_REDRAW;
    message.data = NULL;
    send_message(message);
    wid->ishided = FALSE;
}

inline void switch_touch(int id){
	widget *wid = get_widget_from_id(id);
	if(wid->state_info.button.isholded == FALSE){
		return;
	}

	messages msg;
	msg.dest = id;
	msg.msgid = WIDGET_SWITCH_TOUCH;
	msg.data = NULL;
	send_message(msg);
	wid->state_info.button.isholded = FALSE;
}

inline void data_refresh(int id, void *data){
	messages msg;
	msg.dest = id;
	msg.msgid = DATA_REFRESH;
	msg.data = data;
	send_message(msg);
}

/*                          *
 *        Base Part         *
 *                          */

ret_code create_user_interface_from_array(const widget_info *pawidget, int length){
    int i, ret;
    ret = OK;
    for (i = 0; i < length; ++i)
    {
        if(ret != ERROR){
            ret = create_widget(pawidget[i].id, pawidget[i].x, pawidget[i].y, pawidget[i].width,
                pawidget[i].height, pawidget[i].data, pawidget[i].callback);
        }else{
            return ERROR;
        }
    }
    return OK;
}

ret_code create_widget(int id, int x, int y, int width, int height, char *data, void (*callback)(messages)){
    widget *widget_tmp;
    widget_tmp = (widget *)malloc(sizeof(widget));
    // Initial Data
    widget_tmp->id = id;
    widget_tmp->frame.origin.x = x;
    widget_tmp->frame.origin.y = y;
    widget_tmp->frame.size.width = width;
    widget_tmp->frame.size.height = height;
    widget_tmp->data = data;
    widget_tmp->callback = callback;
    widget_tmp->bound.x.min = x;
    widget_tmp->bound.x.max = x + width;
    widget_tmp->bound.y.min = y;
    widget_tmp->bound.y.max = y + height;
    widget_tmp->ishided = FALSE;
    widget_tmp->state_info.ptr = NULL;	     //this should be use at WIDGET_INIT

    // add to the list
    if(global_widget_table.length < MAX_LIST_LENGTH){
        global_widget_table.list[global_widget_table.length] = *widget_tmp;
        global_widget_table.length++;
    }else{
#ifdef DEBUG_DUMP
        printf("can't alloc the widget.");
#endif
        return ERROR;
    }
    free(widget_tmp);

    //init the widget
    messages message;
    message.dest = id;
    message.msgid = WIDGET_INIT;
    message.data = NULL;
    send_message(message);
    return OK;
}

widget* get_widget_from_id(int id){
    int i;
    for (i = 0; i < global_widget_table.length; i++) {
        if(global_widget_table.list[i].id == id){
            return &global_widget_table.list[i];
        }
    }
#ifdef DEBUG_DUMP
    printf("NO Such Widget!\n");
#endif
    return NULL;
}

void send_message(messages message){
    widget *dest;
    dest = get_widget_from_id(message.dest);
    // Only unhide widget and redraw msg can be called
    if(dest != NULL && (dest->ishided == FALSE || message.msgid == WIDGET_REDRAW)){
        dest->callback(message);
    }else{
#ifdef DEBUG_DUMP
        printf("Wrong Destination when sending message. id:%d\n", message.dest);
#endif
    }
}

void touch_screen_routine(){
    int x, y, none_touch;
    none_touch = 0;
    //get the coordinate of screen
    x = GUI_TOUCH_X_MeasureX();
    y = GUI_TOUCH_X_MeasureY();
    if(x > GUI_TOUCH_AD_RIGHT || x < GUI_TOUCH_AD_LEFT){
    	none_touch = 1;
    }
    if(y > GUI_TOUCH_AD_BOTTOM || y < GUI_TOUCH_AD_TOP){
    	none_touch = 1;
    }
    x = x * GUI_TOUCH_XSIZE / (GUI_TOUCH_AD_RIGHT - GUI_TOUCH_AD_LEFT);
    y = y * GUI_TOUCH_YSIZE / (GUI_TOUCH_AD_BOTTOM - GUI_TOUCH_AD_TOP);
#if GUI_TOUCH_MIRROR_X
    x = GUI_TOUCH_XSIZE - x;
#endif
#if GUI_TOUCH_MIRROR_Y
    y = GUI_TOUCH_YSIZE - y;
#endif
#if GUI_TOUCH_SWAP_XY
    int tmp;
    tmp = x;
    x = y;
    y = tmp;
#endif

    //find the component touched and send message utill it released
    static int last_touched_widget_id = -1;
    int i;

    //force axis setting
    if(none_touch == 1){
    	x = -1;
    	y = -1;
    }
    for(i = 0; i < global_widget_table.length; i++){
    	if(global_widget_table.list[i].ishided == TRUE){
    		continue;
    	}
        if(global_widget_table.list[i].bound.x.min < x && x < global_widget_table.list[i].bound.x.max &&
            global_widget_table.list[i].bound.y.min < y && y < global_widget_table.list[i].bound.y.max){
        	// if be touched
            last_touched_widget_id = global_widget_table.list[i].id;
        }else if (last_touched_widget_id == global_widget_table.list[i].id){
        	// until it released
            last_touched_widget_id = -1;
            messages message;
            message.dest = global_widget_table.list[i].id;
            message.msgid = WIDGET_TOUCHED;
            message.data = NULL;
            send_message(message);
        }
    }
}

void TS_ISR(void *context){
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE, 0);
	touch_screen_routine();

}

void SUI_Init(color c_background){
	LCD_Init();
    TS_Init();
    LCD_light(80);
    bound  bounds;
    bounds.x.min = 0;
    bounds.x.max = 799;
    bounds.y.min = 0;
    bounds.y.max = 479;
	fill_area(bounds, c_background);

	alt_u32 period = 50000000 / 60;

	// set the counter
	IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_0_BASE,  period & 0xFFFF);
	IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_0_BASE, (period & 0xFFFF0000) >> 16);

#ifdef DEBUG_DUMP
	printf("TIMER_PERIOD_H_%d_\n", IORD_ALTERA_AVALON_TIMER_PERIODH(TIMER_0_BASE));
	printf("TIMER_PERIOD_L_%d_\n", IORD_ALTERA_AVALON_TIMER_PERIODL(TIMER_0_BASE));
#endif

	// enable timer
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE, 0x07);
	// set ISR
	alt_ic_isr_register(TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_0_IRQ, TS_ISR, NULL, NULL);
}
