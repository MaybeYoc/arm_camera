#include <stdio.h>

#include <zjpeg.h>  /**/
#include <jpeglib.h>



int yuyv_to_rgb(unsigned char* yuv,unsigned char *a_rgb)
{
    unsigned int i;    
    unsigned char* y0 = yuv + 0;       
    unsigned char* u0 = yuv + 1;    
    unsigned char* y1 = yuv + 2;     
    unsigned char* v0 = yuv + 3;

    unsigned  char* r0 = a_rgb + 0;     
    unsigned  char* g0 = a_rgb + 1;   
    unsigned  char* b0 = a_rgb + 2;    
    unsigned  char* r1 = a_rgb + 3;    
    unsigned  char* g1 = a_rgb + 4;    
    unsigned  char* b1 = a_rgb + 5;   
    float rt0 = 0, gt0 = 0, bt0 = 0, rt1 = 0, gt1 = 0, bt1 = 0;    
    for(i=0;i < video_buf[v4l2_buf.index].length;i=i+4)
    {        
        bt0 = 1.164 * (*y0 - 16) + 2.018 * (*u0 - 128);         
        gt0 = 1.164 * (*y0 - 16) - 0.813 * (*v0 - 128) - 0.394 * (*u0 - 128);       
        rt0 = 1.164 * (*y0 - 16) + 1.596 * (*v0 - 128);          
        bt1 = 1.164 * (*y1 - 16) + 2.018 * (*u0 - 128);       
        gt1 = 1.164 * (*y1 - 16) - 0.813 * (*v0 - 128) - 0.394 * (*u0 - 128);     
        rt1 = 1.164 * (*y1 - 16) + 1.596 * (*v0 - 128);                     
        if(rt0 > 250)  
            rt0 = 255;      
        if(rt0< 0)    
            rt0 = 0;  
        if(gt0 > 250)   
            gt0 = 255;      
        if(gt0 < 0)
            gt0 = 0;     
        if(bt0 > 250)
            bt0 = 255;      
        if(bt0 < 0)
            bt0 = 0; 
        if(rt1 > 250)
            rt1 = 255;      
        if(rt1 < 0)
            rt1 = 0;  
        if(gt1 > 250)
            gt1 = 255;      
        if(gt1 < 0)
            gt1 = 0;   
        if(bt1 > 250)
            bt1 = 255;  
        if(bt1 < 0)
            bt1 = 0;    

        *r0 = (unsigned char)rt0;   
        *g0 = (unsigned char)gt0;       
        *b0 = (unsigned char)bt0;    

        *r1 = (unsigned char)rt1;       
        *g1 = (unsigned char)gt1;   
        *b1 = (unsigned char)bt1;    

        yuv = yuv + 4;       
        a_rgb = a_rgb + 6;        
        if(yuv == NULL)        
        break; 

        y0 = yuv;     
        u0 = yuv + 1; 
        y1 = yuv + 2;  
        v0 = yuv + 3;

        r0 = a_rgb + 0;       
        g0 = a_rgb + 1;        
        b0 = a_rgb + 2;       
        r1 = a_rgb + 3;       
        g1 = a_rgb + 4;       
        b1 = a_rgb + 5; 
    }   
    return 0;
} 

long rgb_to_jpeg(unsigned char *rgb_buf, unsigned char **jpeg_buf) 
{   
    long unsigned jpeg_size = 0;
    struct jpeg_compress_struct jcs;  
    struct jpeg_error_mgr jem;       
    jcs.err = jpeg_std_error(&jem);   
    jpeg_create_compress(&jcs);

    jpeg_mem_dest(&jcs, jpeg_buf, &jpeg_size); 
    JSAMPROW row_pointer[1];    

    int row_stride;         
    jcs.image_width = 640;  
    jcs.image_height = 480;     
    jcs.input_components = 3; 
    jcs.in_color_space = JCS_RGB;   
    jpeg_set_defaults(&jcs);    
    jpeg_set_quality(&jcs, 75, TRUE);    
    row_stride =jcs.image_width * 3; 
    jpeg_start_compress(&jcs, TRUE); 

    while(jcs.next_scanline < jcs.image_height)
    {
        row_pointer[0] = &rgb_buf[(jcs.next_scanline) * row_stride];        
        (void)jpeg_write_scanlines(&jcs, row_pointer, 1);   
    }

    jpeg_finish_compress(&jcs); 
    jpeg_destroy_compress(&jcs);

    return jpeg_size;
}

