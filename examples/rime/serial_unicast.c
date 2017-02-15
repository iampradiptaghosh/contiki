 #include "contiki.h"
 #include "dev/serial-line.h"
 #include <stdio.h>
 #include "net/rime/rime.h"

 PROCESS(test_serial, "Serial line test process");
 AUTOSTART_PROCESSES(&test_serial);
 static void
recv_uc(struct unicast_conn *c, const linkaddr_t *from)
{
  printf("%s\n",(char *)packetbuf_dataptr());
}
/*---------------------------------------------------------------------------*/
static void
sent_uc(struct unicast_conn *c, int status, int num_tx)
{
  const linkaddr_t *dest = packetbuf_addr(PACKETBUF_ADDR_RECEIVER);
  if(linkaddr_cmp(dest, &linkaddr_null)) {
    return;
  }
  printf("unicast message sent to %d.%d: status %d num_tx %d\n",
    dest->u8[0], dest->u8[1], status, num_tx);
}
/*---------------------------------------------------------------------------*/
static const struct unicast_callbacks unicast_callbacks = {recv_uc, sent_uc};
static struct unicast_conn uc;

 PROCESS_THREAD(test_serial, ev, data)
 {
   PROCESS_BEGIN();
   unicast_open(&uc, 146, &unicast_callbacks);

    // serial_line_init();
    // uart0_set_input(serial_line_input_byte);
   for(;;) {
            //printf("test \n");
            // static strusct etimer et;
            // linkaddr_t addr;
            linkaddr_t addr;

            PROCESS_YIELD();


            if(ev == serial_line_event_message) {
              printf("received line: %s\n", (char *)data);
              packetbuf_copyfrom(data, 2);
              addr.u8[0] = 0xed;
              addr.u8[1] = 0x5e;
              if(!linkaddr_cmp(&addr, &linkaddr_node_addr)) {
                  unicast_send(&uc, &addr);
              }
            }
   }
   PROCESS_END();
 }