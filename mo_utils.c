#include "mo_utils.h"

enum
{
    CM_DEC          =   0x0001,
    CM_HEX          =   0x0002,
    CM_OCT          =   0x0004,
    CM_NEWLINE      =   0x0008,
    CM_SPACE        =   0x0010,
    CM_ALPHA        =   0x0020,
    CM_STRING_FLAG  =   0x0040,
    CM_COMMENT_FLAG =   0x0080,
};

static unsigned short mo_cm[256] =
{
/*  0x00    ???    0    */  0,
/*  0x01    ???    1    */  0,
/*  0x02    ???    2    */  0,
/*  0x03    ???    3    */  0,
/*  0x04    ???    4    */  0,
/*  0x05    ???    5    */  0,
/*  0x06    ???    6    */  0,
/*  0x07    ???    7    */  0,
/*  0x08    ???    8    */  0,
/*  0x09    HT     9    */  CM_SPACE,
/*  0x0A    ???   10    */  CM_NEWLINE | CM_STRING_FLAG | CM_COMMENT_FLAG,
/*  0x0B    VT    11    */  CM_SPACE,
/*  0x0C    ???   12    */  0,
/*  0x0D    ???   13    */  0,
/*  0x0E    ???   14    */  0,
/*  0x0F    ???   15    */  0,
/*  0x10    ???   16    */  0,
/*  0x11    ???   17    */  0,
/*  0x12    ???   18    */  0,
/*  0x13    ???   19    */  0,
/*  0x14    ???   20    */  0,
/*  0x15    ???   21    */  0,
/*  0x16    ???   22    */  0,
/*  0x17    ???   23    */  0,
/*  0x18    ???   24    */  0,
/*  0x19    ???   25    */  0,
/*  0x1A    ???   26    */  0,
/*  0x1B    ???   27    */  0,
/*  0x1C    ???   28    */  0,
/*  0x1D    ???   29    */  0,
/*  0x1E    ???   30    */  0,
/*  0x1F    ???   31    */  0,
/*  0x20          32    */  0,
/*  0x21    !     33    */  0,
/*  0x22    "     34    */  CM_STRING_FLAG,
/*  0x23    #     35    */  0,
/*  0x24    $     36    */  0,
/*  0x25    %     37    */  0,
/*  0x26    &     38    */  0,
/*  0x27    '     39    */  0,
/*  0x28    (     40    */  0,
/*  0x29    )     41    */  0,
/*  0x2A    *     42    */  CM_COMMENT_FLAG,
/*  0x2B    +     43    */  0,
/*  0x2C    ,     44    */  0,
/*  0x2D    -     45    */  0,
/*  0x2E    .     46    */  0,
/*  0x2F    /     47    */  0,
/*  0x30    0     48    */  CM_DEC,
/*  0x31    1     49    */  CM_DEC,
/*  0x32    2     50    */  CM_DEC,
/*  0x33    3     51    */  CM_DEC,
/*  0x34    4     52    */  CM_DEC,
/*  0x35    5     53    */  CM_DEC,
/*  0x36    6     54    */  CM_DEC,
/*  0x37    7     55    */  CM_DEC,
/*  0x38    8     56    */  CM_DEC,
/*  0x39    9     57    */  CM_DEC,
/*  0x3A    :     58    */  0,
/*  0x3B    ;     59    */  0,
/*  0x3C    <     60    */  0,
/*  0x3D    =     61    */  0,
/*  0x3E    >     62    */  0,
/*  0x3F    ?     63    */  0,
/*  0x40    @     64    */  0,
/*  0x41    A     65    */  CM_ALPHA | CM_HEX,
/*  0x42    B     66    */  CM_ALPHA | CM_HEX,
/*  0x43    C     67    */  CM_ALPHA | CM_HEX,
/*  0x44    D     68    */  CM_ALPHA | CM_HEX,
/*  0x45    E     69    */  CM_ALPHA | CM_HEX,
/*  0x46    F     70    */  CM_ALPHA | CM_HEX,
/*  0x47    G     71    */  CM_ALPHA,
/*  0x48    H     72    */  CM_ALPHA,
/*  0x49    I     73    */  CM_ALPHA,
/*  0x4A    J     74    */  CM_ALPHA,
/*  0x4B    K     75    */  CM_ALPHA,
/*  0x4C    L     76    */  CM_ALPHA,
/*  0x4D    M     77    */  CM_ALPHA,
/*  0x4E    N     78    */  CM_ALPHA,
/*  0x4F    O     79    */  CM_ALPHA,
/*  0x50    P     80    */  CM_ALPHA,
/*  0x51    Q     81    */  CM_ALPHA,
/*  0x52    R     82    */  CM_ALPHA,
/*  0x53    S     83    */  CM_ALPHA,
/*  0x54    T     84    */  CM_ALPHA,
/*  0x55    U     85    */  CM_ALPHA,
/*  0x56    V     86    */  CM_ALPHA,
/*  0x57    W     87    */  CM_ALPHA,
/*  0x58    X     88    */  CM_ALPHA,
/*  0x59    Y     89    */  CM_ALPHA,
/*  0x5A    Z     90    */  CM_ALPHA,
/*  0x5B    [     91    */  0,
/*  0x5C    \     92    */  CM_STRING_FLAG,
/*  0x5D    ]     93    */  0,
/*  0x5E    ^     94    */  0,
/*  0x5F    _     95    */  CM_ALPHA,
/*  0x60    `     96    */  0,
/*  0x61    a     97    */  CM_ALPHA | CM_HEX,
/*  0x62    b     98    */  CM_ALPHA | CM_HEX,
/*  0x63    c     99    */  CM_ALPHA | CM_HEX,
/*  0x64    d    100    */  CM_ALPHA | CM_HEX,
/*  0x65    e    101    */  CM_ALPHA | CM_HEX,
/*  0x66    f    102    */  CM_ALPHA | CM_HEX,
/*  0x67    g    103    */  CM_ALPHA,
/*  0x68    h    104    */  CM_ALPHA,
/*  0x69    i    105    */  CM_ALPHA,
/*  0x6A    j    106    */  CM_ALPHA,
/*  0x6B    k    107    */  CM_ALPHA,
/*  0x6C    l    108    */  CM_ALPHA,
/*  0x6D    m    109    */  CM_ALPHA,
/*  0x6E    n    110    */  CM_ALPHA,
/*  0x6F    o    111    */  CM_ALPHA,
/*  0x70    p    112    */  CM_ALPHA,
/*  0x71    q    113    */  CM_ALPHA,
/*  0x72    r    114    */  CM_ALPHA,
/*  0x73    s    115    */  CM_ALPHA,
/*  0x74    t    116    */  CM_ALPHA,
/*  0x75    u    117    */  CM_ALPHA,
/*  0x76    v    118    */  CM_ALPHA,
/*  0x77    w    119    */  CM_ALPHA,
/*  0x78    x    120    */  CM_ALPHA,
/*  0x79    y    121    */  CM_ALPHA,
/*  0x7A    z    122    */  CM_ALPHA,
/*  0x7B    {    123    */  0,
/*  0x7C    |    124    */  0,
/*  0x7D    }    125    */  0,
/*  0x7E    ~    126    */  0,
/*  0x7F         127    */  0,
/*  0x80         128    */  0,
/*  0x81         129    */  0,
/*  0x82         130    */  0,
/*  0x83         131    */  0,
/*  0x84         132    */  0,
/*  0x85         133    */  0,
/*  0x86         134    */  0,
/*  0x87         135    */  0,
/*  0x88         136    */  0,
/*  0x89         137    */  0,
/*  0x8A         138    */  0,
/*  0x8B         139    */  0,
/*  0x8C         140    */  0,
/*  0x8D         141    */  0,
/*  0x8E         142    */  0,
/*  0x8F         143    */  0,
/*  0x90         144    */  0,
/*  0x91         145    */  0,
/*  0x92         146    */  0,
/*  0x93         147    */  0,
/*  0x94         148    */  0,
/*  0x95         149    */  0,
/*  0x96         150    */  0,
/*  0x97         151    */  0,
/*  0x98         152    */  0,
/*  0x99         153    */  0,
/*  0x9A         154    */  0,
/*  0x9B         155    */  0,
/*  0x9C         156    */  0,
/*  0x9D         157    */  0,
/*  0x9E         158    */  0,
/*  0x9F         159    */  0,
/*  0xA0         160    */  0,
/*  0xA1         161    */  0,
/*  0xA2         162    */  0,
/*  0xA3         163    */  0,
/*  0xA4         164    */  0,
/*  0xA5         165    */  0,
/*  0xA6         166    */  0,
/*  0xA7         167    */  0,
/*  0xA8         168    */  0,
/*  0xA9         169    */  0,
/*  0xAA         170    */  0,
/*  0xAB         171    */  0,
/*  0xAC         172    */  0,
/*  0xAD         173    */  0,
/*  0xAE         174    */  0,
/*  0xAF         175    */  0,
/*  0xB0         176    */  0,
/*  0xB1         177    */  0,
/*  0xB2         178    */  0,
/*  0xB3         179    */  0,
/*  0xB4         180    */  0,
/*  0xB5         181    */  0,
/*  0xB6         182    */  0,
/*  0xB7         183    */  0,
/*  0xB8         184    */  0,
/*  0xB9         185    */  0,
/*  0xBA         186    */  0,
/*  0xBB         187    */  0,
/*  0xBC         188    */  0,
/*  0xBD         189    */  0,
/*  0xBE         190    */  0,
/*  0xBF         191    */  0,
/*  0xC0         192    */  0,
/*  0xC1         193    */  0,
/*  0xC2         194    */  0,
/*  0xC3         195    */  0,
/*  0xC4         196    */  0,
/*  0xC5         197    */  0,
/*  0xC6         198    */  0,
/*  0xC7         199    */  0,
/*  0xC8         200    */  0,
/*  0xC9         201    */  0,
/*  0xCA         202    */  0,
/*  0xCB         203    */  0,
/*  0xCC         204    */  0,
/*  0xCD         205    */  0,
/*  0xCE         206    */  0,
/*  0xCF         207    */  0,
/*  0xD0         208    */  0,
/*  0xD1         209    */  0,
/*  0xD2         210    */  0,
/*  0xD3         211    */  0,
/*  0xD4         212    */  0,
/*  0xD5         213    */  0,
/*  0xD6         214    */  0,
/*  0xD7         215    */  0,
/*  0xD8         216    */  0,
/*  0xD9         217    */  0,
/*  0xDA         218    */  0,
/*  0xDB         219    */  0,
/*  0xDC         220    */  0,
/*  0xDD         221    */  0,
/*  0xDE         222    */  0,
/*  0xDF         223    */  0,
/*  0xE0         224    */  0,
/*  0xE1         225    */  0,
/*  0xE2         226    */  0,
/*  0xE3         227    */  0,
/*  0xE4         228    */  0,
/*  0xE5         229    */  0,
/*  0xE6         230    */  0,
/*  0xE7         231    */  0,
/*  0xE8         232    */  0,
/*  0xE9         233    */  0,
/*  0xEA         234    */  0,
/*  0xEB         235    */  0,
/*  0xEC         236    */  0,
/*  0xED         237    */  0,
/*  0xEE         238    */  0,
/*  0xEF         239    */  0,
/*  0xF0         240    */  0,
/*  0xF1         241    */  0,
/*  0xF2         242    */  0,
/*  0xF3         243    */  0,
/*  0xF4         244    */  0,
/*  0xF5         245    */  0,
/*  0xF6         246    */  0,
/*  0xF7         247    */  0,
/*  0xF8         248    */  0,
/*  0xF9         249    */  0,
/*  0xFA         250    */  0,
/*  0xFB         251    */  0,
/*  0xFC         252    */  0,
/*  0xFD         253    */  0,
/*  0xFE         254    */  0,
/*  0xFF         255    */  0,
};


MO_EXTERN mo_byte* mo_lex_skipspace(struct lex_t*  x, mo_byte* pc)
{
    ///!    �������еĿհ�
    x->cache->pos = pc;
    while (mo_cm[(int)*pc] & CM_SPACE) {
        pc++;
    }

    x->cache->pos = pc;
    return x->cache->pos;
}


MO_EXTERN mo_byte* mo_lex_load_more(struct lex_t* x, struct result_t* r, mo_byte* pc)
{
    struct cache_t* cache = x->cache;

    //  �ȱ�����pos��λ��
    cache->pos = pc;

    //  ���������̫���ڿ�����������β������ô�������ƶ�һ��
    if ((cache->limit - cache->pos) < cache->rsrv) {
        int offset = cache->pos - cache->buf;
        int len = cache->end - cache->pos;
        if (0 > len) {
            memmove(cache->buf, cache->pos, len);
        }
        if (offset > 0) {
            cache->pos = cache->buf;
            cache->end = cache->pos + len;
            cache->end[0] = '\n';
            cache->anchor->line -= offset;
        }
    }

RETRY:
    //  ���������ٶ�ȡ��������
    int ret = x->stream->read(x->stream->ctx, &(cache->end), cache->limit);
    if (MO_READ_OK == ret) {
        return cache->pos;
    }

    if (MO_READ_EOF == ret) {
        struct stream_t* old_stream = x->stream;
        x->stream = x->stream->prev;
        old_stream->close(old_stream->ctx);
        if (NULL == x->stream) {
            return cache->pos;
        }
        goto RETRY;
    }

    mo_result_errorf(r, 111, "read from stream failed");
    return cache->pos;
}

//  ��λ����һ����ע�ͷǿհ״�
static mo_byte* mo_lex_locate(struct lex_t* x, struct result_t* r, mo_byte* pc)
{
    register struct cache_t*    cache;

RETRY:
    cache = x->cache;
    pc = cache->pos;

    //  �������еĿհ�
    pc = mo_lex_skipspace(x, pc);
    
    //  ���Ԥ����������̫�٣���ô������������
    if ((cache->end - pc) < cache->rsrv) {
        pc = mo_lex_load_more(x, r, pc);
    }

    //  �����������,��Ҫ�жϵ����������滻�л����������ǻ����������ݱ���ȡ����
    if (*pc != '\n') {
        cache->pos = pc; ///<    �Ƚ��Ѿ����Թ��Ŀհ�����
        return MO_READ_OK;
    }

    //  ���ȷʵ�������ı�����
    if (pc != cache->end) {
        mo_lex_newline(x, r, pc);
        goto RETRY;
    }

    //  �ӻ�������ȡ��������
    pc = mo_lex_load_more(x, r, pc);

    //  �����ȡʧ�ܣ���ô����
    if (!mo_result_ok(r)) {
        return cache->pos;
    }

    //  �����ȡ�ɹ����������ݲ�û�����ӣ�˵�������ļ�β��ֱ�ӷ��ؼ���
    if (cache->pos == cache->end) {
        return cache->pos;
    }

    //  �����ȡ���ݳɹ������������ӣ���ô��������һ��
    goto RETRY;
}


MO_EXTERN   mo_byte*            mo_lex_newline(struct lex_t* x, struct result_t* r, mo_byte* pc)
{
    x->cache->anchor->lino++;  ///<    ������һ��
    x->cache->anchor->line = pc + 1;
    x->cache->pos = pc + 1;
    return x->cache->pos;
}

MO_EXTERN   mo_byte*            mo_lex_singleline_comment(struct lex_t* x, struct result_t* r, mo_byte* pc, int pervsize, mo_byte escape_newline)
{
    struct cache_t* cache = x->cache;
    pc = pc + pervsize;
    int escape_open = MO_FALSE;
    while (mo_result_ok(r)) {
        if (*pc == '\n') {
            //  ���pc���Ļ��в������滻�У����ǻ��������ݽ�����ǣ���ô���������������
            if (pc == cache->end) {
                pc = mo_lex_load_more(x, r, pc);
                //  ���¼������ݺ�ָ��λ��û�б仯��˵��û�и���������
                if (pc == cache->end) {
                    cache->pos = pc;
                    return cache->pos;
                }

                //  ����Ѿ������˸�������ݣ���ô������ǰ��ʶ�����
                continue;
            }

            //  ����˵���������滻�У���ʱ���һ�»��б��
            mo_lex_newline(x, r, pc);

            //  ����պ������˻��з�ת����ţ���ôע�ͱ�ÿ��Կ���
            if (MO_TRUE == escape_open) {
                escape_open = MO_FALSE;
                continue;
            }

            //  ����Ǹ���������Ļ��У���ô˵����������β������ע�Ϳ��Խ�����
            return cache->pos;
        }

        if (*pc == escape_newline) {
            escape_open = MO_TRUE;
        } else {
            escape_open = MO_FALSE;
        }
        pc++;
    }
}

static mo_byte*            mo_lex_accept_number_hex(struct lex_t* x, struct token_t* k, struct result_t* r, mo_byte* pc)
{

}

static mo_byte*            mo_lex_accept_number_dec(struct lex_t* x, struct token_t* k, struct result_t* r, mo_byte* pc)
{

}

static mo_byte*            mo_lex_accept_number_oct(struct lex_t* x, struct token_t* k, struct result_t* r, mo_byte* pc)
{

}

static mo_byte*            mo_lex_accept_number_float_postfix(struct lex_t* x, struct token_t* k, struct result_t* r, mo_byte* pc)
{

}

MO_EXTERN   mo_byte*            mo_lex_accept_number(struct lex_t* x, struct token_t* k, struct result_t* r, mo_byte* pc)
{
    if ('0' == *pc) {
        //  �������ʮ������ǰ׺
        if (('x' == pc[1]) || ('X' == pc[1])) {
            return mo_lex_accept_number_hex(x, k, r, pc);
        }

        //  �����������8����ǰ׺
        if (mo_cm[pc[1]] & (CM_OCT)) {
            return mo_lex_accept_number_oct(x, k, r, pc);
        }

        //  ����������ϸ���ʶ���
        if (('.' == pc[1]) || ('e' == pc[1]) || ('E' == pc[1])) {
            return mo_lex_accept_number_float_postfix(x, k, r, pc);
        }

        //  �����������ĸ���������������������ʽ����
        if (mo_cm[pc[1]] & (CM_ALPHA)) {
            mo_result_errorf(r, 111, "");
            return pc;
        }

        //  ȫ���ų��������Ϊʮ��������ʶ�����������ʵ��ֻ�и�0
        return mo_lex_accept_number_dec(x, k, r, pc);
    }

    //  �����ʼ�ַ�����ʮ�������֣���ô����ʮ����������ʶ��
    if (mo_cm[*pc] & (CM_DEC)) {
        return mo_lex_accept_number_dec(x, k, r, pc);
    }

    //  ����������Ǹ������ĵ��ǰ׺����ô������������ʶ��
    if ('.' == *pc) {
        return mo_lex_accept_number_float_postfix(x, k, r, pc);
    }

    //  �޷�ʶ��ķ���
    mo_result_errorf(r, 111, "");
    return pc;
}
