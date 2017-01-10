//Collin Dreher
#include <stdio.h>
#include <string.h>

//Set up structure for image header.
typedef struct
{
	unsigned short start_Marker, app1_Marker, app1_Length;
	char exifString[4];
	unsigned short NUL_Marker;
	char endianness[2];
	unsigned short version_Number;
	unsigned int offset;
}Header;

//Set up structure for TIFF Tags.
typedef struct
{
	unsigned short tag_id, data_type;
	unsigned int num_data_items, offset;
}Tag;

int check(Header header)
{
	//Print an error if the Exif string isn't in the right place.
	if(0 != strcmp(header.exifString, "Exif"))
	{
		printf("The tag was not found within the file.");
		return 0;
	}
	
	//Print an error if the TIFF header contains MM instead of II.
	if(strcmp(header.endianness, "II*") != 0)
	{
		printf("We do not support big endianness! Sorry.");
		return 0;
	}
	
	return 1;
}

void printOtherTags(FILE *f, int count)
{
	//Declare variables for other TIFF tags.
	unsigned int i, temp, value1, value2, result;
	unsigned short pixelWidth = 0xA002, pixelHeight = 0xA003, ISO_Speed = 0x8827;
	unsigned short exposureSpeed = 0x829a, F_Stop = 0x829d, lensFocalLength = 0x920A, date = 0x9003;
	char str[100];
	Tag tag;
	
	for(i = 0; i < count; i++)
	{
		//Read tags.
		fread(&tag, sizeof(tag), 1, f);
		
		//Obtain current value of file position.
		temp = ftell(f);
		
		//Get and print exposure speed.
		if(exposureSpeed == tag.tag_id)
		{
			fseek(f, tag.offset + 12, SEEK_SET);
			fread(&value1, sizeof(value1), 1, f);
			fread(&value2, sizeof(value1), 1, f);
			printf("Exposure Time: %d/%d seconds\n", value1, value2);
		}
		//Get and print F-stop.
		else if(F_Stop == tag.tag_id)
		{
			fseek(f, tag.offset + 12, SEEK_SET);
			fread(&value1, sizeof(value1), 1, f);
			fread(&value2, sizeof(value1), 1, f);
			printf("F-stop: f/%.1f \n", (double)value1/value2);
		}
		//Get and print ISO speed.
		else if(ISO_Speed == tag.tag_id)
		{
			printf("ISO: ISO %d\n", tag.offset);
		}
		//Get and print date taken.
		else if(date == tag.tag_id)
		{
			fseek(f, tag.offset + 12, SEEK_SET);
			fread(&str, sizeof(str[0]), tag.num_data_items, f);
			printf("Date: %s\n", str);
		}
		//Get and print focal length of the lens.
		else if(lensFocalLength == tag.tag_id)
		{
			fseek(f, tag.offset + 12, SEEK_SET);
			fread(&value1, sizeof(value1), 1, f);
			fread(&value2, sizeof(value1), 1, f);
			printf("Focal Length: %.0f mm\n", (double)(value1 / value2));
		}
		//Get and print width in pixels.
		else if(pixelWidth == tag.tag_id)
		{
			printf("Width: %d pixels\n", tag.offset);
		}
		//Get and print height in pixels.
		else if(pixelHeight == tag.tag_id)
		{
			printf("Height: %d pixels\n", tag.offset);
		}
		
		//Set file position.
		fseek(f, temp, SEEK_SET);
	}
}

int main(int argc, char *argv[])
{
	FILE *f;
	Header header;
	Tag tag;
	
	//Declare variables, such as TIFF tags.
	unsigned short count, otherTagCount;
	unsigned short manString = 0x010f, modelString = 0x0110, exifSubBlock = 0x08769;
	unsigned int i, temp;
	char str[100];
	
	
	f = fopen(argv[1], "r");
	//Read in image header.
	fread(&header, sizeof(header), 1, f);
	
	//Check header field.
	check(header);
	
	fread(&count, sizeof(count), 1, f);
	
	for(i = 0; i < count; i++)
	{
		//Read tags.
		fread(&tag, sizeof(tag), 1, f);
		
		//Obtain current value of file position.
		temp = ftell(f);
		
		//Once first TIFF tag is reached, get and display manufacturer string.
		if(manString == tag.tag_id)
		{
			fseek(f, tag.offset + 12, SEEK_SET);
			fread(&str, sizeof(str[0]), tag.num_data_items, f);
			printf("Manufacturer: %s\n", str);
		}
		//Once second TIFF tag is reached, get and display camera model string.
		else if(modelString == tag.tag_id)
		{
			fseek(f, tag.offset + 12, SEEK_SET);
			fread(&str, sizeof(str[0]), tag.num_data_items, f);
			printf("Model: %s\n", str);
		}
		//Get and display exif sub block address.
		else if(exifSubBlock == tag.tag_id)
		{
			fseek(f, tag.offset + 12, SEEK_SET);
			fread(&otherTagCount, sizeof(otherTagCount), tag.num_data_items, f);
			printOtherTags(f, otherTagCount);
		}
		
		//Set file position.
		fseek(f, temp, SEEK_SET);
	}
}
