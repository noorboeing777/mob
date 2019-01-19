/* Symbol stack definition */
struct SymbolE 
    {
    STPtr ptr;
    struct SymbolE *next;
    };
extern struct SymbolE *SymbolS_top;


/* Type stack definition */
struct TypeE 
    {
    enum Types type;
    struct TypeE *next;
    };
extern struct TypeE *TypeS_top;


/* Count stack definition */

struct CountE 
    {
    int itemCount;
    struct CountE *next;
    };

extern struct CountE *CountS_top;

