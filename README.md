# lkcp

Lua bindings for `https://github.com/skywind3000/kcp`

# Install

use `luarocks install lkcp`

# Interface

* lkcp_create(conv, func)
* lkcp_recv()
* lkcp_send(data)
* lkcp_update(current)
* lkcp_check(current)
* lkcp_input(data)
* lkcp_flush()
* lkcp_wndsize(sndwnd, rcvwnd)
* lkcp_nodelay(nodelay, interval, resend, nc)
* lkcp_peeksize()
* lkcp_setmtu(mtu)
* lkcp_waitsnd()
* lkcp_logmask()