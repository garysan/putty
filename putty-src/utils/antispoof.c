#include "putty.h"
#include "misc.h"

void seat_antispoof_msg(InteractionReadySeat iseat, const char *msg)
{
    strbuf *sb = strbuf_new();
    seat_set_trust_status(iseat.seat, true);
    if (seat_can_set_trust_status(iseat.seat)) {
        /*
         * If the seat can directly indicate that this message is
         * generated by the client, then we can just use the message
         * unmodified as an unspoofable header.
         */
        put_dataz(sb, msg);
    } else if (*msg) {
        /*
         * Otherwise, add enough padding around it that the server
         * wouldn't be able to mimic it within our line-length
         * constraint.
         */
        put_fmt(sb, "-- %s ", msg);
        while (sb->len < 78)
            put_byte(sb, '-');
    }
    put_datapl(sb, PTRLEN_LITERAL("\r\n"));
    seat_banner_pl(iseat, ptrlen_from_strbuf(sb));
    strbuf_free(sb);
}
