#include <highgui.h>
#include <cv.h>
#include <stdio.h>
#include <conio.h>
#include <wingdi.h>

char program_path[200];
char program_path2[200];

void HBITMAP2BMP(HBITMAP hbit,char *Path)

{

    BITMAPFILEHEADER fh;

    BITMAPINFOHEADER ih;

    BITMAP bit;

    BITMAPINFO *pih;

    int PalSize;

    HANDLE hFile;

    DWORD dwWritten,Size;

    HDC hdc;



    hdc=GetDC(NULL);



    GetObject(hbit,sizeof(BITMAP),&bit);

    ih.biSize=sizeof(BITMAPINFOHEADER);

    ih.biWidth=bit.bmWidth;

    ih.biHeight=bit.bmHeight;

    ih.biPlanes=1;

    ih.biBitCount=bit.bmPlanes*bit.bmBitsPixel;

    if (ih.biBitCount > 8) ih.biBitCount=24;

    ih.biCompression=BI_RGB;

    ih.biSizeImage=0;

    ih.biXPelsPerMeter=0;

    ih.biYPelsPerMeter=0;

    ih.biClrUsed=0;

    ih.biClrImportant=0;



    PalSize=(ih.biBitCount==24 ? 0:1 << ih.biBitCount)*sizeof(RGBQUAD);

    pih=(BITMAPINFO *)malloc(ih.biSize+PalSize);

    pih->bmiHeader=ih;



    GetDIBits(hdc,hbit,0,bit.bmHeight,NULL,pih,DIB_RGB_COLORS);

    ih=pih->bmiHeader;



    if (ih.biSizeImage == 0) {

        ih.biSizeImage=((((ih.biWidth*ih.biBitCount)+31) & ~31) >> 3) * ih.biHeight;

    }



    Size=ih.biSize+PalSize+ih.biSizeImage;

    pih=(BITMAPINFO *)realloc(pih,Size);



    GetDIBits(hdc,hbit,0,bit.bmHeight,(PBYTE)pih+ih.biSize+PalSize,pih,DIB_RGB_COLORS);



    fh.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+PalSize;

    fh.bfReserved1=0;

    fh.bfReserved2=0;

    fh.bfSize=Size+sizeof(BITMAPFILEHEADER);

    fh.bfType=0x4d42;



    hFile=CreateFile(Path,GENERIC_WRITE,0,NULL,

        CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

    WriteFile(hFile,&fh,sizeof(fh),&dwWritten,NULL);

    WriteFile(hFile,pih,Size,&dwWritten,NULL);



    ReleaseDC(NULL,hdc);

    CloseHandle(hFile);

}


int load2Clip(void)
{
    HBITMAP hBit;
    if(IsClipboardFormatAvailable(CF_BITMAP)){
        OpenClipboard(NULL);

        hBit = (HBITMAP)GetClipboardData(CF_BITMAP);
//        CloseClipboard();

        HBITMAP2BMP(hBit,"angnai.bmp");
    }
}

void Convert_Char(IplImage *src)
{
    CvScalar Color1,imgColor,setColor;
    CvSize size;
    long i,j,k,l,line,left,right;

   // cvShowImage("before",src);
    
    setColor.val[0] = 255;
    setColor.val[1] = 255;
    setColor.val[2] = 255;
    size = cvGetSize(src);
    for(i=0;i<size.height;i++){
        Color1 = cvGet2D(src,i,3);
        for(j=0;j<size.width;j++){
            imgColor = cvGet2D(src,i,j);
            if(ScalarComp(imgColor,Color1)) cvSet2D(src,i,j,setColor);
        }
    }


    Color1.val[0] = 0;
    Color1.val[1] = 0;
    Color1.val[2] = 0;
    for(i=0;i<size.height;i++){
        for(j=0;j<size.width;j++){
            imgColor = cvGet2D(src,i,j);
            // 흰색이 아닌 것을 찾았다면 검정색으로 변경함.
            if(!ScalarComp(imgColor,setColor)) cvSet2D(src,i,j,Color1);
        }
    }
}

int matchImage(char *arg1, char *arg2)
{
    IplImage *src,*dest,*result;
    double min,max;
    CvPoint lefttop;

    dest = cvLoadImage(arg1,-1);        // 캡쳐 이미지
    src = cvLoadImage(arg2,-1);         // 찾을 이미지
    
    int width = dest->width - src->width + 1;
    int height = dest->height - src->height + 1;
    result = cvCreateImage(cvSize(width,height),IPL_DEPTH_32F,1);//= cvLoadImage(arg1,-1);      // 저장 이미지
    
    cvMatchTemplate(dest,src,result,CV_TM_CCOEFF_NORMED);
    cvMinMaxLoc(result,&min,&max,NULL,&lefttop,NULL);
    cvRectangle(dest, lefttop , cvPoint(lefttop.x+src->width,lefttop.y+src->height) , CV_RGB(0,0,0) , 3 , NULL, NULL);
    
    
    
    cvReleaseImage(&src);
    cvReleaseImage(&result);
    
    // 보안문자의 시작위치
    lefttop.x = lefttop.x-65;
    lefttop.y = lefttop.y-163;
    // 보안문자 Size = 276x63
    width = 276;
    height = 63;
    
    src = cvCreateImage(cvSize(width,height),IPL_DEPTH_32F,1); // 저장 이미지
    cvSetImageROI(dest, cvRect(lefttop.x,lefttop.y,width,height));
    
    src = cvClone(dest);//cvCopy(dest,src,NULL);
    
    cvResetImageROI(dest);
    cvReleaseImage(&dest);
    

    int p[3];
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    cvSaveImage("out_org.png",src,p);

    Convert_Char(src);
    
    cvSaveImage("out_result.png",src,p);
    cvSaveImage(program_path2,src,p);
    
    cvReleaseImage(&src);
    
    cvDestroyAllWindows();
    
    return 0;
}

int print2clip(char *source)
{
    int i;
    int ok = OpenClipboard(NULL);
    if (!ok) return 0;


    for(i=0;i<10;i++){
        if((source[i] >= 'A') && (source[i] <= 'Z'));
        else if((source[i] >= 'a') && (source[i] <= 'z'));
        else{
            source[i] = source[i+1];
            source[i+1] = ' ';
        }
    }
    
    for(i=0;i<10;i++){
        if((source[i] >= 'A') && (source[i] <= 'Z'));
        else if((source[i] >= 'a') && (source[i] <= 'z'));
        else{
            source[i] = 0;
        }
    }


    HGLOBAL clipbuffer;
    char * buffer;



    EmptyClipboard();
    clipbuffer = GlobalAlloc(GMEM_DDESHARE, strlen(source)+1);
    buffer = (char*)GlobalLock(clipbuffer);
    strcpy(buffer, source);
    GlobalUnlock(clipbuffer);
    SetClipboardData(CF_TEXT,clipbuffer);
    CloseClipboard();



    return 1;
}
int ScalarComp(CvScalar color1,CvScalar color2)
{
    int i;
    for(i=0;i<3;i++){
        if(color1.val[0] != color2.val[0]) return 0;
        if(color1.val[1] != color2.val[1]) return 0;
        if(color1.val[2] != color2.val[2]) return 0;
    }
    return 1;
}

int main(int argc,char *argv[])
{
    FILE *fp;
    char sVal[10];
    int i;


    fp = fopen("path.txt","r");
    fgets(program_path,200,fp);
    fclose(fp);
    
    // tesseract.exe
    // out_result.png
    memcpy(program_path2,program_path,strlen(program_path));
    sprintf(program_path,"%stesseract.exe",program_path);
    sprintf(program_path2,"%sout_result.png",program_path2);
    
    load2Clip();

    matchImage("angnai.bmp","Input.png");
    
    PROCESS_INFORMATION pi;

    STARTUPINFO si;

    si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;

    si.wShowWindow = SW_SHOW;

    
    ShellExecute(NULL,"open",program_path,"out_result.png test1 -l eng",NULL,SW_SHOW);

    while(1){
        if(!access("test1.txt",0)) break;
    }
    
    fp = fopen("test1.txt","r");
    fread(sVal,10,1,fp);
    fclose(fp);
    
    print2clip(sVal);
    
    system("del test1.txt");

    return 0;
}
