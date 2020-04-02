#ifndef SIMPLEBITMAP_TEMPLATE_24783_IS_INCLUDED
#define SIMPLEBITMAP_TEMPLATE_24783_IS_INCLUDED

template <class ComponentType,int NumComponentPerPixel>
class SimpleBitmapTemplate
{
private:
	typedef SimpleBitmapTemplate <ComponentType,NumComponentPerPixel> THISCLASS;

	SimpleBitmapTemplate(const THISCLASS &incoming);
	THISCLASS &operator=(const THISCLASS &incoming);

private:
	int nx,ny;
	ComponentType *bmpPtr;

protected:
	/*! Returns pointer to top of the horizontal line. */
	ComponentType *LineTop(int y)
	{
		// &bmpPtr[0]                          ->First line
		// &bmpPtr[nx*NumComponentPerPixel]    ->3nd line
		// &bmpPtr[2*nx*NumComponentPerPixel]  ->2nd line
		return bmpPtr+y*nx*NumComponentPerPixel;
	}
	const ComponentType *LineTop(int y) const
	{
		return bmpPtr+y*nx*NumComponentPerPixel;
	}

public:
	SimpleBitmapTemplate()
	{
		nx=0;
		ny=0;
		bmpPtr=nullptr;
	}
	~SimpleBitmapTemplate()
	{
		CleanUp();
	}
	void CleanUp(void)
	{
		nx=0;
		ny=0;
		if(nullptr!=bmpPtr)
		{
			delete [] bmpPtr;
			bmpPtr=nullptr;
		}
	}

	/*! This function, you will populate in the assignment. 
	    This function cuts out part of the bitmap into destination bitmap.
	    The part on this bitmap is (thisX0,thisY0)-(thisX0+wid-1,thisY0+hei-1).
	    If a pixel coordinate in this bitmap that is to be copied to the destination is outside of
	    this bitmap, all components of the destination pixel must be clearColor.
	*/
	void CutOut(SimpleBitmapTemplate <ComponentType,NumComponentPerPixel> &destination,int thisX0,int thisY0,int wid,int hei,ComponentType clearColor) const
	{
		destination.Create(wid, hei);
		// auto dst = destination.GetEditableBitmapPointer();
		// auto dst = destination.GetPixelPointer((i-thisX0), (j-thisY0));
		for (int i=thisX0; i<thisX0+hei; i++)
		{
			// printf("\n\n -- %d", i);
			// printf("\n");
			for (int j=thisY0; j<thisY0+wid; j++)
			{
				auto dst = destination.GetEditablePixelPointer(i-thisX0,j-thisY0);
				// printf("\t\tsbmp: %d\n", j-thisY0);
				auto check = this->IsInRange(i,j) == true;
				// printf("\n------------%d-----------", check);
				if (this->IsInRange(i,j) == true)
				{
					auto src = this->GetPixelPointer(i, j);
					
					for (int k=0; k<NumComponentPerPixel; k++)
					{
						// printf("\n%d\n",src[k]);
						dst[k] = src[k];
						// printf("\n%d\n",dst[k]);
					}
					
				}

				else
				{
					for (int k=0; k<NumComponentPerPixel; k++)
					{
						dst[k] = clearColor;
					}
				}
							

			}
		}
		// auto dst = destination.GetBitmapPointer()
		// for (int i=0; i<wid; i++)
		// {
		// 	for (int j=0; j<hei; j++)
		// 	{
		// 		// dst = dst + NumComponentPerPixel*(j*nx + i);
		// 		dst = GetPixelPointer(i,j);
		// 		dst[i][j] = bmpPtr[thisX0+i][thisY0+j];
		// 	}
		// }
	}

	/*! Returns the total number of components in the bitmap of given width and height.
	    If, the number of components per pixel is 4, (100x100) bitmap has 4x100x100 total number of components. */
	static int GetTotalNumComponent(int wid,int hei)
	{
		return NumComponentPerPixel*wid*hei;
	}

	/*! Returns the total number of components in this bitmap. */
	int GetTotalNumComponent(void) const
	{
		return GetTotalNumComponent(nx,ny);
	}

	/*! Returns the total number of components in one single line for the given width.
	    If the width of the bitmap is 100, and the total number of components per pixel is 4,
	    4x100 components per line. */
	static int GetNumComponentPerLine(int wid)
	{
		return NumComponentPerPixel*wid;
	}

	/*! Returns the total number of components in one single line for this bitmap. */
	int GetNumComponentPerLine(void) const
	{
		return GetNumComponentPerLine(nx);
	}

	/*! Make a copy of the incoming bitmap. */
	SimpleBitmapTemplate <ComponentType,NumComponentPerPixel> &CopyFrom(const SimpleBitmapTemplate <ComponentType,NumComponentPerPixel> &incoming)
	{
		if(&incoming!=this)
		{
			CleanUp();
			this->nx=incoming.nx;
			this->ny=incoming.ny;
			this->bmpPtr=new ComponentType [GetTotalNumComponent(incoming.nx,incoming.ny)];
			for(int i=0; i<GetTotalNumComponent(incoming.nx,incoming.ny); ++i)
			{
				this->bmpPtr[i]=incoming.bmpPtr[i];
			}
		}
		return *this;
	}

	/*! Moves the incoming bitmap to this bitmap. */
	SimpleBitmapTemplate <ComponentType,NumComponentPerPixel> &MoveFrom(SimpleBitmapTemplate <ComponentType,NumComponentPerPixel> &incoming)
	{
		if(&incoming!=this)
		{
			CleanUp();
			this->nx=incoming.nx;
			this->ny=incoming.ny;
			this->bmpPtr=incoming.bmpPtr;
			incoming.nx=0;
			incoming.ny=0;
			incoming.bmpPtr=nullptr;
		}
		return *this;
	}

	/*! Creates a bitmap. */
	bool Create(int wid,int hei)
	{
		CleanUp();
		this->nx=wid;
		this->ny=hei;
		this->bmpPtr=new ComponentType [GetTotalNumComponent(wid,hei)];
		return true;
	}

	/*! Returns the width in the number of pixels. */
	int GetWidth(void) const
	{
		return nx;
	}

	/*! Returns the height in the number of pixels. */
	int GetHeight(void) const
	{
		return ny;
	}

	/*! Returns the pointer to the bitmap. */
	const ComponentType *GetBitmapPointer(void) const
	{
		return bmpPtr;
	}

	/*! Returns the pointer to the pixel. */
	const ComponentType *GetPixelPointer(int x,int y) const
	{
		return bmpPtr+NumComponentPerPixel*(y*nx+x);
	}

	/*! Returns the mutable pointer to the bitmap. */
	ComponentType *GetEditableBitmapPointer(void)
	{
		return bmpPtr;
	}

	/*! Returns the mutable pointer to the pixel. */
	ComponentType *GetEditablePixelPointer(int x,int y)
	{
		return bmpPtr+NumComponentPerPixel*(y*nx+x);
	}

	/*! Flips the image upside down. */
	void Invert(void)
	{
		for(int y=0; y<ny/2; ++y)
		{
			auto L0=LineTop(y);
			auto L1=LineTop(ny-1-y);
			for(int x=0; x<GetNumComponentPerLine(); ++x)
			{
				auto a=L0[x];
				L0[x]=L1[x];
				L1[x]=a;
			}
		}
	}

	/*! Sets an array and width and height dirctory.
	    After this function, this class takes an ownership of incomingBmpPtr.
	    It must not be deleted outside of this class. */
	void SetDirect(int wid,int hei,ComponentType *incomingBmpPtr)
	{
		nx=wid;
		ny=hei;
		bmpPtr=incomingBmpPtr;
	}

	/*! Returns true if (x,y) is inside the bitmap. */
	bool IsInRange(int x,int y) const
	{
		return (0<=x && x<nx && 0<=y && y<ny);
	}

	/*! Returns true if 0<=x && x<GetWidth(). */
	bool IsXInRange(int x) const
	{
		return (0<=x && x<nx);
	}

	/*! Returns true if 0<=y && y<GetHeight(). */
	bool IsYInRange(int y) const
	{
		return (0<=y && y<ny);
	}
};

#endif
