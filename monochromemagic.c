#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>


#pragma pack(push,1)  // am aflat de #pragma de pe urmatorul link: https://stackoverflow.com/questions/57134763/generate-monochrome-bitmap-image-using-c

typedef struct{
  uint16_t type;
  uint32_t size;
  uint16_t reserved1;
  uint16_t reserved2;
  uint32_t offset;
  uint32_t headersize;
  uint32_t width;
  uint32_t height;
  uint16_t planes;
  uint16_t bitsperpixel;
  uint32_t compresionmethod;
  uint32_t imagesize;
  uint32_t hresolution;
  uint32_t vresolution;
  uint32_t colorpalete;
  uint32_t nrimportantcolors;
}Header;

#pragma pack (pop)

int main(int argc,char **argv)
{
  FILE *fin=fopen(argv[1],"rb");
  if(fin==NULL)
    {
      perror("eroare");
      exit(-1);
    }
  FILE *fout=fopen(argv[2],"wb");
  if(fout==NULL)
    {
      perror("eroare");
      exit(-1);
    }
  Header bmp;
  fread(&bmp,sizeof(Header),1,fin);
  fwrite(&bmp,sizeof(Header),1,fout);
  uint8_t *data=malloc((bmp.width)*3*sizeof(uint8_t));
  if(data==NULL)
    {
      perror("eroare");
      exit(-1);
    }
  for(int i=0;i<bmp.height;i++)
    {
      fread(data,(bmp.width)*3*sizeof(uint8_t),1,fin);
      for(int j=0;j<bmp.width;j++)
	{
	  uint8_t gri=(data[j*3]+data[j*3+1]+data[j*3+2])/3;
	  data[j*3]=gri;
	  data[j*3+1]=gri;
	  data[j*3+2]=gri;
	}
      fwrite(data,3*(bmp.width)*sizeof(uint8_t),1,fout);
    }
  if(fclose(fin)!=0)
    {
      perror("eroare");
      exit(-1);
    }
  free(data);
  if(fclose(fout)!=0)
    {
      perror("eroare");
      exit(-1);
    }
  return 0;
} 
