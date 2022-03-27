#include "../nes.h"
#include <ctype.h>

//#define USE_SPLIT
//#define USE_VRAMBUF
//#define USE_READVRAM
//#define USE_RLE
#define USE_FADE
#define USE_OFF_SCROLL

void put_str(const uint16_t adr, const char* str) 
{
    vram_adr(adr);
    vram_write((const uint8_t*)str, strlen(str));
}

//!< オフスクリーンスクロール
uint8_t ScrollNT[NT_TILE_HEIGHT];
uint8_t ScrollAT[sizeof(ScrollNT) >> 2];
#define NTADR_VERT(x, y) x < 32 ? NTADR_A(x, y) : NTADR_B(x & 31, y)
#define NTADR_HORZ(x, y) y < 30 ? NTADR_A(x, y) : NTADR_B(x, y % 30)
//!< 垂直ミラーリングでは
//!< 0 ---- 32 ---- 63
//!<    A        B

void __fastcall__ irq_nmi_callback(void) 
{
  // check high bit of A to see if this is an IRQ
  if (__A__ & 0x80) {
    // it's an IRQ from the MMC3 mapper
    // change PPU scroll registers
   // PPU.scroll = counters[irqcount & 0x7f] >> 8;
   // PPU.scroll = 0;
    // advance to next scroll value
    //++irqcount;
    // acknowledge interrupt
    MMC3_IRQ_DISABLE();
    MMC3_IRQ_ENABLE();
  } else {
    // this is a NMI
    // reload IRQ counter
    MMC3_IRQ_RELOAD();
    // reset scroll counter
    //irqcount = 0;
  }
}

void main()
{
    const char Palette[] = {
        0x03, 0x11, 0x30, 0x27, //!< BG Pal0
        0x00, 0x1c, 0x20, 0x2c, //!< BG Pal1
        0x00, 0x00, 0x10, 0x20, //!< BG Pal2
        0x00, 0x06, 0x16, 0x26, //!< BG Pal3

        0x00, 0x16, 0x35, 0x24, //!< SP Pal0
        0x00, 0x00, 0x37, 0x25, //!< SP Pal1
        0x00, 0x0d, 0x2d, 0x3a, //!< SP Pal2
        0x00, 0x0d, 0x27, 0x2a, //!< SP Pal3
    };
    #ifdef USE_RLE
    #define RLE_TAG 0x1
    const uint8_t NameTableRLE[] = { 
        RLE_TAG, 
        //!< データ, タグ, 個数,
        0x16, RLE_TAG, 0xff, //!< 255
        0x17, RLE_TAG, 0xff, //!< 510
        0x18, RLE_TAG, 0xff, //!< 765
        0x19, RLE_TAG, 0xc3, //!< 960 == 32 * 30
        0x0,
    };
    #endif
    const uint8_t AttributeTable[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 
        0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 
        0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 
    };

    int16_t scrX, scrY;
    uint8_t i;
    uint8_t oam_id;
    uint8_t fade = 0;

    //!< 割り込み
    {
        MMC3_IRQ_SET_VALUE(7);
        MMC3_IRQ_RELOAD();
        MMC3_IRQ_ENABLE();
        ENABLE_CPU_IRQ;
        nmi_set_callback(irq_nmi_callback);
    }

    //pal_bg(BGPalette);
    //pal_spr(SPRPalette);
    pal_all(Palette);

    //!< ネームテーブル
    {
        //!< PPUオフの時に行う
        vram_adr(NAMETABLE_A);
#ifdef USE_RLE
        //!< REL(Run Length Encoding) 解凍して読み込む場合
        vram_unrle(NameTableRLE);
        //!< LZ4 解凍して読み込む場合
        //vram_unlz4();
#else
        vram_fill(0x16, 32 * 30);
#endif
        //!< アトリビュートテーブル 
        //vram_adr(NAMETABLE_A + 0x3c0); //!< 連続でやる場合はアドレスが進んでいるので必要ない (0x3c0 == 960 == 32 * 30)
        vram_write(AttributeTable, sizeof(AttributeTable));


        //!< 書き込み先の VRAM として NTADR_A(x, y)(==ネームテーブルA(x, y)) のアドレスを指定
        put_str(NTADR_A(2, 2), "Hello World");

        put_str(NTADR_A(2, 0), "A00");
        put_str(NTADR_A(2, NT_TILE_HEIGHT >> 1), "A15");
        put_str(NTADR_A(2, NT_TILE_HEIGHT - 1), "A29");

        put_str(NTADR_C(2, 0), "C00");
        put_str(NTADR_C(2, NT_TILE_HEIGHT >> 1), "C15");
        put_str(NTADR_C(2, NT_TILE_HEIGHT - 1), "C29");
    }
    
    oam_clear();
#ifdef USE_SPLIT
    //!< スプライト 0 と BG のピクセルが衝突した時に PPU_STATUS レジスタがセットされる、エッジを隠しているため x + 8 している
    oam_spr(1 + 8, 30, 0xa0, OAM_BEHIND, 0);
#endif

    //!< 描画をオンにする
    //ppu_on_all();
    //ppu_on_bg();
    //ppu_on_spr();
    //!< モノクロ
    //ppu_mask(MASK_BG | MASK_SPR | MASK_MONO);
    //!< 淡色
    //ppu_mask(MASK_BG | MASK_SPR | (MASK_TINT_RED | MASK_TINT_GREEN | MASK_TINT_BLUE));
    //!< エッジを隠さない設定
    //ppu_mask(MASK_BG | MASK_SPR | MASK_EDGE_BG | MASK_EDGE_SPR);
    //!< 左側、上側のエッジを隠す設定にしておく
    ppu_mask(MASK_BG | MASK_SPR);

    {
        VRAMBUF_CLEAR;
        //!< VRAM バッファのアドレスを知らせておく
        set_vram_update(VramBuffer);
#ifdef USE_VRAMBUF
        vrambuf_put_h(NTADR_A(2, 16), "1234567890");
        vrambuf_put_h(NTADR_A(2, 17), "ABCDEFGHIJ");

        vrambuf_put_v(NTADR_A(16, 2), "1234567890");
        vrambuf_put_v(NTADR_A(17, 2), "ABCDEFGHIJ");
#endif
    }

#ifdef USE_FADE
    //!< フェード (黒から開始)
    pal_bright(0);
#endif

    scrX = scrY = 0;
    while(1) {
        //!< パッド
        {
            const uint8_t pre0 = pad_state(0);
            const uint8_t cur0 = pad_poll(0);
        
            if(cur0 & PAD_LEFT) { --scrX; }
            if(cur0 & PAD_RIGHT) { ++scrX; }
            //if((cur0 & PAD_LEFT) ^ (pre0 & PAD_LEFT)) { --scrX; }
            //if((cur0 & PAD_RIGHT) ^ (pre0 & PAD_RIGHT)) { ++scrX; }
            if(cur0 & PAD_UP) { --scrY; }
            if(cur0 & PAD_DOWN) { ++scrY; }
            while(scrX > NT_WIDTH2) { scrX -= NT_WIDTH2; }
            while(scrX < 0) { scrX += NT_WIDTH2; }
            while(scrY > NT_HEIGHT2) { scrY -= NT_HEIGHT2; }
            while(scrY < 0) { scrY += NT_HEIGHT2; }
        }

#ifdef USE_OFF_SCROLL
        //vrambuf_put_v(NTADR_VERT(scrX, scrY), ScrollNT, sizeof(ScrollNT));
#endif

        //!< スプライト
        {
            oam_id = 4;
            for(i = 0;i < 32;++i) {
                uint8_t x = (i << 3); // + (NT_WIDTH >> 1);
                uint8_t y = (i << 3); // + (NT_HEIGHT >> 1);
                //!< OAM バッファにスプライトを書き込む
                oam_id = oam_spr(x, y, 16 + i, 0, oam_id);
            }
            if(oam_id) {
                oam_hide_rest(oam_id);
            }
        }

#ifdef USE_FADE
        //!< フェード [0, 8] ... 0==黒, 4==通常色, 8==白
        {
            fade = MIN(++fade, 64);
            pal_bright(fade >> 4);
            //pal_bg_bright(), pal_spr_bright();
        }
#endif

#ifdef USE_VRAMBUF
        vrambuf_put(NTADR_A(2, 28), "abcdefghijklmnopqrstuvwxyz", NT_UPD_HORZ);
#endif

#ifdef USE_SPLIT
        //!< スプライト 0 コリジョンを待ってから、x スクロール値をセットする、y スクロール値は無視される
        split(scrX, 0);
#else
        scroll(scrX, scrY);
#endif
        //!< PAL, NTSC を問わず、次のフレームまでウエイト
        ppu_wait_nmi();

#ifdef USE_READVRAM
        {
            uint8_t x = 2 + (rand8() & 15);
            uint8_t read;
            vram_adr(NTADR_A(x, 2));
            vram_read(&read, sizeof(read));
            //!< VRAM を読み込んでアルファベットだったら、大文字かどうか("U" or "L")を次の行に出力
            if(isalpha(read)){
                put_str(NTADR_A(x, 3), isupper(read) ? "U" : "L");
            }
            //!< VRAM アドレスは戻しておく必要がある
            vram_adr(0x0);
        }
#endif
    }

    //!< 描画をオフにする
    ppu_off();
}