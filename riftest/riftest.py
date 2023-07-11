import argparse
import pexpect

name = "root"


def ssh(host):
    return "ssh %s@%s" % (name, host)


def conn(host, password):
    child = pexpect.spawn(ssh(host), timeout=3)

    child.setwinsize(400, 400)

    index = child.expect([pexpect.TIMEOUT, "yes/no", "password:"])
    if index == 0:
        return True, None

    if index == 1:
        child.sendline("yes")
        index = child.expect([pexpect.TIMEOUT, "yes/no", "password:"])

    if index == 2:
        child.sendline(password)

    return None, child


def check_capture(child, args):
    index = child.expect([pexpect.TIMEOUT, "#"], timeout=3)
    if index == 1:
        child.sendline("tcpdump -eni {} -c 10".format(args.ifname))
    else:
        return True, "FAILED to login"

    index = child.expect([pexpect.TIMEOUT, "10 packets captured"], timeout=3)
    if index == 0:
        return True, "Failed to capture"

    return None, "SUCCESS"


def run(args):
    result = []
    for host in args.hosts:
        res = {}
        res["Host"] = host
        print("Check [{}] >>".format(host))

        err, child = conn(host, args.password)
        if err:
            print("  Failed to conn ", host)
            res["result"] = "Failed"
            result.append(res)
            continue

        print("  SUCCESS to connect. ", host)

        err, reason = check_capture(child, args)
        if err:
            res["result"] = "Failed"
            res["reason"] = reason
        else:
            res["result"] = "OK"
            res["reason"] = reason

        result.append(res)
        child.close()

    print(result)


def main():
    parser = argparse.ArgumentParser(description="Check interface")
    parser.add_argument(
        "-i",
        metavar="Host IP",
        dest="hosts",
        action="append",
        help="Host IP",
        required=True,
    )
    parser.add_argument(
        "-p",
        metavar="password",
        dest="password",
        action="store",
        help="ssh password",
        required=True,
    )
    parser.add_argument(
        "--if",
        metavar="interface",
        dest="ifname",
        action="store",
        help="interface name",
        required=True,
    )
    args = parser.parse_args()

    run(args)


if __name__ == "__main__":
    main()
