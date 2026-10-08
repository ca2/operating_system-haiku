#include "platform.h"
#include "image.h"
#include "bitmap.h"
#include "aura/graphics/image/image_pixmap_lease.h"
#include "acme/graphics/image/pixmap.h"
namespace draw2d_haiku {
::image_pixmap_lease image::_map(::image::enum_map mode,const ::i32_rectangle &r){
 if(!m_pdraw2dbitmap)return ::image::image::_map(mode,r);
 auto *b=dynamic_cast<bitmap *>(m_pdraw2dbitmap.m_p);if(!b)throw ::exception(error_wrong_state);
 _tidy_map(r);b->read_pixels();auto p=create_newø<::pixmap>();
 p->m_memoryPixmap.reference_data(b->m_memoryDraw2dBitmap.data(),b->m_memoryDraw2dBitmap.size());
 p->m_pimage32Raw=(::image32_t *)b->m_memoryDraw2dBitmap.data();p->m_iScan=b->m_size.cx*4;
 p->m_bTopLeft=true;p->m_size=m_size;p->m_sizeRaw=b->m_size;m_ppixmapOwned=p;
 p->pixmap_map(r.is_set()?r : ::i32_rectangle(m_point,m_size));return {this,p};
}
void image::_unmap(::image_pixmap_lease *lease){
 ::image::image::_unmap(lease);if(auto *b=dynamic_cast<bitmap *>(m_pdraw2dbitmap.m_p))b->commit_pixels();
}
}
