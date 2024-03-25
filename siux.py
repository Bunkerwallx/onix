import re
import socket
import sshuttle.helpers as helpers
import sshuttle.client as client
import sshuttle.firewall as firewall
import sshuttle.hostwatch as hostwatch
import sshuttle.ssyslog as ssyslog
from sshuttle.options import parser, parse_ipport
from sshuttle.helpers import family_ip_tuple, log, Fatal
from sshuttle.sudoers import sudoers

def main():
    opt = parser.parse_args()

    if opt.sudoers_no_modify:
        sudoers(user_name=opt.sudoers_user)

    if opt.daemon:
        opt.syslog = 1

    if opt.wrap:
        import sshuttle.ssnet as ssnet
        ssnet.MAX_CHANNEL = opt.wrap

    if opt.latency_buffer_size:
        import sshuttle.ssnet as ssnet
        ssnet.LATENCY_BUFFER_SIZE = opt.latency_buffer_size

    helpers.verbose = opt.verbose

    try:
        if opt.firewall:
            if opt.subnets or opt.subnets_file:
                parser.error('exactly zero arguments expected')
            return firewall.main(opt.method, opt.syslog)
        
        elif opt.hostwatch:
            hostwatch.hw_main(opt.subnets, opt.auto_hosts)
            return 0
        
        else:
            includes = [item for sublist in opt.subnets + opt.subnets_file for item in sublist]
            excludes = [item for sublist in opt.exclude for item in sublist]

            if not includes and not opt.auto_nets:
                parser.error('at least one subnet, subnet file, or -N expected')

            remotename = opt.remote if opt.remote not in ['', '-'] else None
            nslist = [family_ip_tuple(ns) for ns in opt.ns_hosts]
            sh = re.split(r'[\s,]+', (opt.seed_hosts or "").strip()) if opt.seed_hosts else [] if opt.auto_hosts else None

            ipport_v4, ipport_v6 = None, None
            if opt.listen:
                ipport_v4, ipport_v6 = parse_ipport(ip) for ip in opt.listen.split(",")

            opt.tmark = opt.tmark.lower()
            if not opt.tmark.startswith("0x"):
                opt.tmark = "0x%s" % opt.tmark

            if opt.syslog:
                ssyslog.start_syslog()
                ssyslog.close_stdin()
                ssyslog.stdout_to_syslog()
                ssyslog.stderr_to_syslog()

            return_code = client.main(ipport_v6, ipport_v4, opt.ssh_cmd, remotename, opt.python, opt.latency_control,
                                      opt.latency_buffer_size, opt.dns, nslist, opt.method, sh, opt.auto_hosts, opt.auto_nets,
                                      includes, excludes, opt.daemon, opt.to_ns, opt.pidfile, opt.user, opt.sudo_pythonpath, opt.tmark)

            log('Normal exit code, exiting...') if return_code == 0 else log('Abnormal exit code %d detected, failing...' % return_code)
            return return_code

    except Fatal as e:
        log('fatal: %s' % e)
        return 99

    except KeyboardInterrupt:
        log('\n')
        log('Keyboard interrupt: exiting.')
        return 1
