
bufbomb:     file format elf64-x86-64


Disassembly of section .init:

0000000000401000 <_init>:
  401000:	f3 0f 1e fa          	endbr64
  401004:	48 83 ec 08          	sub    $0x8,%rsp
  401008:	48 8b 05 d1 2f 00 00 	mov    0x2fd1(%rip),%rax        # 403fe0 <__gmon_start__@Base>
  40100f:	48 85 c0             	test   %rax,%rax
  401012:	74 02                	je     401016 <_init+0x16>
  401014:	ff d0                	call   *%rax
  401016:	48 83 c4 08          	add    $0x8,%rsp
  40101a:	c3                   	ret

Disassembly of section .plt:

0000000000401020 <.plt>:
  401020:	ff 35 ca 2f 00 00    	push   0x2fca(%rip)        # 403ff0 <_GLOBAL_OFFSET_TABLE_+0x8>
  401026:	ff 25 cc 2f 00 00    	jmp    *0x2fcc(%rip)        # 403ff8 <_GLOBAL_OFFSET_TABLE_+0x10>
  40102c:	0f 1f 40 00          	nopl   0x0(%rax)
  401030:	f3 0f 1e fa          	endbr64
  401034:	68 00 00 00 00       	push   $0x0
  401039:	e9 e2 ff ff ff       	jmp    401020 <_init+0x20>
  40103e:	66 90                	xchg   %ax,%ax
  401040:	f3 0f 1e fa          	endbr64
  401044:	68 01 00 00 00       	push   $0x1
  401049:	e9 d2 ff ff ff       	jmp    401020 <_init+0x20>
  40104e:	66 90                	xchg   %ax,%ax
  401050:	f3 0f 1e fa          	endbr64
  401054:	68 02 00 00 00       	push   $0x2
  401059:	e9 c2 ff ff ff       	jmp    401020 <_init+0x20>
  40105e:	66 90                	xchg   %ax,%ax
  401060:	f3 0f 1e fa          	endbr64
  401064:	68 03 00 00 00       	push   $0x3
  401069:	e9 b2 ff ff ff       	jmp    401020 <_init+0x20>
  40106e:	66 90                	xchg   %ax,%ax
  401070:	f3 0f 1e fa          	endbr64
  401074:	68 04 00 00 00       	push   $0x4
  401079:	e9 a2 ff ff ff       	jmp    401020 <_init+0x20>
  40107e:	66 90                	xchg   %ax,%ax
  401080:	f3 0f 1e fa          	endbr64
  401084:	68 05 00 00 00       	push   $0x5
  401089:	e9 92 ff ff ff       	jmp    401020 <_init+0x20>
  40108e:	66 90                	xchg   %ax,%ax

Disassembly of section .plt.sec:

0000000000401090 <puts@plt>:
  401090:	f3 0f 1e fa          	endbr64
  401094:	ff 25 66 2f 00 00    	jmp    *0x2f66(%rip)        # 404000 <puts@GLIBC_2.2.5>
  40109a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

00000000004010a0 <printf@plt>:
  4010a0:	f3 0f 1e fa          	endbr64
  4010a4:	ff 25 5e 2f 00 00    	jmp    *0x2f5e(%rip)        # 404008 <printf@GLIBC_2.2.5>
  4010aa:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

00000000004010b0 <strcmp@plt>:
  4010b0:	f3 0f 1e fa          	endbr64
  4010b4:	ff 25 56 2f 00 00    	jmp    *0x2f56(%rip)        # 404010 <strcmp@GLIBC_2.2.5>
  4010ba:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

00000000004010c0 <signal@plt>:
  4010c0:	f3 0f 1e fa          	endbr64
  4010c4:	ff 25 4e 2f 00 00    	jmp    *0x2f4e(%rip)        # 404018 <signal@GLIBC_2.2.5>
  4010ca:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

00000000004010d0 <exit@plt>:
  4010d0:	f3 0f 1e fa          	endbr64
  4010d4:	ff 25 46 2f 00 00    	jmp    *0x2f46(%rip)        # 404020 <exit@GLIBC_2.2.5>
  4010da:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

00000000004010e0 <getc@plt>:
  4010e0:	f3 0f 1e fa          	endbr64
  4010e4:	ff 25 3e 2f 00 00    	jmp    *0x2f3e(%rip)        # 404028 <getc@GLIBC_2.2.5>
  4010ea:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

Disassembly of section .text:

00000000004010f0 <_start>:
  4010f0:	f3 0f 1e fa          	endbr64
  4010f4:	31 ed                	xor    %ebp,%ebp
  4010f6:	49 89 d1             	mov    %rdx,%r9
  4010f9:	5e                   	pop    %rsi
  4010fa:	48 89 e2             	mov    %rsp,%rdx
  4010fd:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
  401101:	50                   	push   %rax
  401102:	54                   	push   %rsp
  401103:	45 31 c0             	xor    %r8d,%r8d
  401106:	31 c9                	xor    %ecx,%ecx
  401108:	48 c7 c7 e1 13 40 00 	mov    $0x4013e1,%rdi
  40110f:	ff 15 c3 2e 00 00    	call   *0x2ec3(%rip)        # 403fd8 <__libc_start_main@GLIBC_2.34>
  401115:	f4                   	hlt
  401116:	66 2e 0f 1f 84 00 00 	cs nopw 0x0(%rax,%rax,1)
  40111d:	00 00 00 

0000000000401120 <_dl_relocate_static_pie>:
  401120:	f3 0f 1e fa          	endbr64
  401124:	c3                   	ret
  401125:	66 2e 0f 1f 84 00 00 	cs nopw 0x0(%rax,%rax,1)
  40112c:	00 00 00 
  40112f:	90                   	nop

0000000000401130 <deregister_tm_clones>:
  401130:	b8 40 40 40 00       	mov    $0x404040,%eax
  401135:	48 3d 40 40 40 00    	cmp    $0x404040,%rax
  40113b:	74 13                	je     401150 <deregister_tm_clones+0x20>
  40113d:	b8 00 00 00 00       	mov    $0x0,%eax
  401142:	48 85 c0             	test   %rax,%rax
  401145:	74 09                	je     401150 <deregister_tm_clones+0x20>
  401147:	bf 40 40 40 00       	mov    $0x404040,%edi
  40114c:	ff e0                	jmp    *%rax
  40114e:	66 90                	xchg   %ax,%ax
  401150:	c3                   	ret
  401151:	66 66 2e 0f 1f 84 00 	data16 cs nopw 0x0(%rax,%rax,1)
  401158:	00 00 00 00 
  40115c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000401160 <register_tm_clones>:
  401160:	be 40 40 40 00       	mov    $0x404040,%esi
  401165:	48 81 ee 40 40 40 00 	sub    $0x404040,%rsi
  40116c:	48 89 f0             	mov    %rsi,%rax
  40116f:	48 c1 ee 3f          	shr    $0x3f,%rsi
  401173:	48 c1 f8 03          	sar    $0x3,%rax
  401177:	48 01 c6             	add    %rax,%rsi
  40117a:	48 d1 fe             	sar    $1,%rsi
  40117d:	74 11                	je     401190 <register_tm_clones+0x30>
  40117f:	b8 00 00 00 00       	mov    $0x0,%eax
  401184:	48 85 c0             	test   %rax,%rax
  401187:	74 07                	je     401190 <register_tm_clones+0x30>
  401189:	bf 40 40 40 00       	mov    $0x404040,%edi
  40118e:	ff e0                	jmp    *%rax
  401190:	c3                   	ret
  401191:	66 66 2e 0f 1f 84 00 	data16 cs nopw 0x0(%rax,%rax,1)
  401198:	00 00 00 00 
  40119c:	0f 1f 40 00          	nopl   0x0(%rax)

00000000004011a0 <__do_global_dtors_aux>:
  4011a0:	f3 0f 1e fa          	endbr64
  4011a4:	80 3d 9d 2e 00 00 00 	cmpb   $0x0,0x2e9d(%rip)        # 404048 <completed.0>
  4011ab:	75 13                	jne    4011c0 <__do_global_dtors_aux+0x20>
  4011ad:	55                   	push   %rbp
  4011ae:	48 89 e5             	mov    %rsp,%rbp
  4011b1:	e8 7a ff ff ff       	call   401130 <deregister_tm_clones>
  4011b6:	c6 05 8b 2e 00 00 01 	movb   $0x1,0x2e8b(%rip)        # 404048 <completed.0>
  4011bd:	5d                   	pop    %rbp
  4011be:	c3                   	ret
  4011bf:	90                   	nop
  4011c0:	c3                   	ret
  4011c1:	66 66 2e 0f 1f 84 00 	data16 cs nopw 0x0(%rax,%rax,1)
  4011c8:	00 00 00 00 
  4011cc:	0f 1f 40 00          	nopl   0x0(%rax)

00000000004011d0 <frame_dummy>:
  4011d0:	f3 0f 1e fa          	endbr64
  4011d4:	eb 8a                	jmp    401160 <register_tm_clones>

00000000004011d6 <valid>:
  4011d6:	f3 0f 1e fa          	endbr64
  4011da:	55                   	push   %rbp
  4011db:	48 89 e5             	mov    %rsp,%rbp
  4011de:	48 83 ec 10          	sub    $0x10,%rsp
  4011e2:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  4011e6:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4011ea:	48 8d 15 17 0e 00 00 	lea    0xe17(%rip),%rdx        # 402008 <_IO_stdin_used+0x8>
  4011f1:	48 89 d6             	mov    %rdx,%rsi
  4011f4:	48 89 c7             	mov    %rax,%rdi
  4011f7:	e8 b4 fe ff ff       	call   4010b0 <strcmp@plt>
  4011fc:	85 c0                	test   %eax,%eax
  4011fe:	0f 94 c0             	sete   %al
  401201:	0f b6 c0             	movzbl %al,%eax
  401204:	c9                   	leave
  401205:	c3                   	ret

0000000000401206 <danger>:
  401206:	f3 0f 1e fa          	endbr64
  40120a:	55                   	push   %rbp
  40120b:	48 89 e5             	mov    %rsp,%rbp
  40120e:	48 8d 05 fd 0d 00 00 	lea    0xdfd(%rip),%rax        # 402012 <_IO_stdin_used+0x12>
  401215:	48 89 c7             	mov    %rax,%rdi
  401218:	e8 73 fe ff ff       	call   401090 <puts@plt>
  40121d:	48 8d 05 0c 0e 00 00 	lea    0xe0c(%rip),%rax        # 402030 <_IO_stdin_used+0x30>
  401224:	48 89 c7             	mov    %rax,%rdi
  401227:	e8 64 fe ff ff       	call   401090 <puts@plt>
  40122c:	90                   	nop
  40122d:	5d                   	pop    %rbp
  40122e:	c3                   	ret

000000000040122f <protect>:
  40122f:	f3 0f 1e fa          	endbr64
  401233:	55                   	push   %rbp
  401234:	48 89 e5             	mov    %rsp,%rbp
  401237:	48 83 ec 10          	sub    $0x10,%rsp
  40123b:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  40123f:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  401243:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401247:	48 89 c7             	mov    %rax,%rdi
  40124a:	e8 87 ff ff ff       	call   4011d6 <valid>
  40124f:	85 c0                	test   %eax,%eax
  401251:	74 11                	je     401264 <protect+0x35>
  401253:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
  401257:	48 89 c7             	mov    %rax,%rdi
  40125a:	b8 00 00 00 00       	mov    $0x0,%eax
  40125f:	e8 a2 ff ff ff       	call   401206 <danger>
  401264:	90                   	nop
  401265:	c9                   	leave
  401266:	c3                   	ret

0000000000401267 <smoke>:
  401267:	f3 0f 1e fa          	endbr64
  40126b:	55                   	push   %rbp
  40126c:	48 89 e5             	mov    %rsp,%rbp
  40126f:	48 8d 05 e3 0d 00 00 	lea    0xde3(%rip),%rax        # 402059 <_IO_stdin_used+0x59>
  401276:	48 89 c7             	mov    %rax,%rdi
  401279:	e8 12 fe ff ff       	call   401090 <puts@plt>
  40127e:	bf 00 00 00 00       	mov    $0x0,%edi
  401283:	e8 48 fe ff ff       	call   4010d0 <exit@plt>

0000000000401288 <fizz>:
  401288:	f3 0f 1e fa          	endbr64
  40128c:	55                   	push   %rbp
  40128d:	48 89 e5             	mov    %rsp,%rbp
  401290:	48 83 ec 10          	sub    $0x10,%rsp
  401294:	89 7d fc             	mov    %edi,-0x4(%rbp)
  401297:	81 7d fc 04 03 02 01 	cmpl   $0x1020304,-0x4(%rbp)
  40129e:	75 1b                	jne    4012bb <fizz+0x33>
  4012a0:	8b 45 fc             	mov    -0x4(%rbp),%eax
  4012a3:	89 c6                	mov    %eax,%esi
  4012a5:	48 8d 05 c8 0d 00 00 	lea    0xdc8(%rip),%rax        # 402074 <_IO_stdin_used+0x74>
  4012ac:	48 89 c7             	mov    %rax,%rdi
  4012af:	b8 00 00 00 00       	mov    $0x0,%eax
  4012b4:	e8 e7 fd ff ff       	call   4010a0 <printf@plt>
  4012b9:	eb 19                	jmp    4012d4 <fizz+0x4c>
  4012bb:	8b 45 fc             	mov    -0x4(%rbp),%eax
  4012be:	89 c6                	mov    %eax,%esi
  4012c0:	48 8d 05 d1 0d 00 00 	lea    0xdd1(%rip),%rax        # 402098 <_IO_stdin_used+0x98>
  4012c7:	48 89 c7             	mov    %rax,%rdi
  4012ca:	b8 00 00 00 00       	mov    $0x0,%eax
  4012cf:	e8 cc fd ff ff       	call   4010a0 <printf@plt>
  4012d4:	bf 00 00 00 00       	mov    $0x0,%edi
  4012d9:	e8 f2 fd ff ff       	call   4010d0 <exit@plt>

00000000004012de <Gets>:
  4012de:	f3 0f 1e fa          	endbr64
  4012e2:	55                   	push   %rbp
  4012e3:	48 89 e5             	mov    %rsp,%rbp
  4012e6:	48 83 ec 20          	sub    $0x20,%rsp
  4012ea:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
  4012ee:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  4012f2:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
  4012f6:	eb 11                	jmp    401309 <Gets+0x2b>
  4012f8:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4012fc:	48 8d 50 01          	lea    0x1(%rax),%rdx
  401300:	48 89 55 f8          	mov    %rdx,-0x8(%rbp)
  401304:	8b 55 f4             	mov    -0xc(%rbp),%edx
  401307:	88 10                	mov    %dl,(%rax)
  401309:	48 8b 05 40 2d 00 00 	mov    0x2d40(%rip),%rax        # 404050 <infile>
  401310:	48 89 c7             	mov    %rax,%rdi
  401313:	e8 c8 fd ff ff       	call   4010e0 <getc@plt>
  401318:	89 45 f4             	mov    %eax,-0xc(%rbp)
  40131b:	83 7d f4 ff          	cmpl   $0xffffffff,-0xc(%rbp)
  40131f:	74 06                	je     401327 <Gets+0x49>
  401321:	83 7d f4 0a          	cmpl   $0xa,-0xc(%rbp)
  401325:	75 d1                	jne    4012f8 <Gets+0x1a>
  401327:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  40132b:	48 8d 50 01          	lea    0x1(%rax),%rdx
  40132f:	48 89 55 f8          	mov    %rdx,-0x8(%rbp)
  401333:	c6 00 00             	movb   $0x0,(%rax)
  401336:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  40133a:	c9                   	leave
  40133b:	c3                   	ret

000000000040133c <bushandler>:
  40133c:	f3 0f 1e fa          	endbr64
  401340:	55                   	push   %rbp
  401341:	48 89 e5             	mov    %rsp,%rbp
  401344:	48 83 ec 10          	sub    $0x10,%rsp
  401348:	89 7d fc             	mov    %edi,-0x4(%rbp)
  40134b:	48 8d 05 66 0d 00 00 	lea    0xd66(%rip),%rax        # 4020b8 <_IO_stdin_used+0xb8>
  401352:	48 89 c7             	mov    %rax,%rdi
  401355:	e8 36 fd ff ff       	call   401090 <puts@plt>
  40135a:	48 8d 05 77 0d 00 00 	lea    0xd77(%rip),%rax        # 4020d8 <_IO_stdin_used+0xd8>
  401361:	48 89 c7             	mov    %rax,%rdi
  401364:	e8 27 fd ff ff       	call   401090 <puts@plt>
  401369:	bf 00 00 00 00       	mov    $0x0,%edi
  40136e:	e8 5d fd ff ff       	call   4010d0 <exit@plt>

0000000000401373 <seghandler>:
  401373:	f3 0f 1e fa          	endbr64
  401377:	55                   	push   %rbp
  401378:	48 89 e5             	mov    %rsp,%rbp
  40137b:	48 83 ec 10          	sub    $0x10,%rsp
  40137f:	89 7d fc             	mov    %edi,-0x4(%rbp)
  401382:	48 8d 05 67 0d 00 00 	lea    0xd67(%rip),%rax        # 4020f0 <_IO_stdin_used+0xf0>
  401389:	48 89 c7             	mov    %rax,%rdi
  40138c:	e8 ff fc ff ff       	call   401090 <puts@plt>
  401391:	48 8d 05 40 0d 00 00 	lea    0xd40(%rip),%rax        # 4020d8 <_IO_stdin_used+0xd8>
  401398:	48 89 c7             	mov    %rax,%rdi
  40139b:	e8 f0 fc ff ff       	call   401090 <puts@plt>
  4013a0:	bf 00 00 00 00       	mov    $0x0,%edi
  4013a5:	e8 26 fd ff ff       	call   4010d0 <exit@plt>

00000000004013aa <illegalhandler>:
  4013aa:	f3 0f 1e fa          	endbr64
  4013ae:	55                   	push   %rbp
  4013af:	48 89 e5             	mov    %rsp,%rbp
  4013b2:	48 83 ec 10          	sub    $0x10,%rsp
  4013b6:	89 7d fc             	mov    %edi,-0x4(%rbp)
  4013b9:	48 8d 05 58 0d 00 00 	lea    0xd58(%rip),%rax        # 402118 <_IO_stdin_used+0x118>
  4013c0:	48 89 c7             	mov    %rax,%rdi
  4013c3:	e8 c8 fc ff ff       	call   401090 <puts@plt>
  4013c8:	48 8d 05 09 0d 00 00 	lea    0xd09(%rip),%rax        # 4020d8 <_IO_stdin_used+0xd8>
  4013cf:	48 89 c7             	mov    %rax,%rdi
  4013d2:	e8 b9 fc ff ff       	call   401090 <puts@plt>
  4013d7:	bf 00 00 00 00       	mov    $0x0,%edi
  4013dc:	e8 ef fc ff ff       	call   4010d0 <exit@plt>

00000000004013e1 <main>:
  4013e1:	f3 0f 1e fa          	endbr64
  4013e5:	55                   	push   %rbp
  4013e6:	48 89 e5             	mov    %rsp,%rbp
  4013e9:	48 83 ec 10          	sub    $0x10,%rsp
  4013ed:	89 7d fc             	mov    %edi,-0x4(%rbp)
  4013f0:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  4013f4:	48 8d 05 78 ff ff ff 	lea    -0x88(%rip),%rax        # 401373 <seghandler>
  4013fb:	48 89 c6             	mov    %rax,%rsi
  4013fe:	bf 0b 00 00 00       	mov    $0xb,%edi
  401403:	e8 b8 fc ff ff       	call   4010c0 <signal@plt>
  401408:	48 8d 05 2d ff ff ff 	lea    -0xd3(%rip),%rax        # 40133c <bushandler>
  40140f:	48 89 c6             	mov    %rax,%rsi
  401412:	bf 07 00 00 00       	mov    $0x7,%edi
  401417:	e8 a4 fc ff ff       	call   4010c0 <signal@plt>
  40141c:	48 8d 05 87 ff ff ff 	lea    -0x79(%rip),%rax        # 4013aa <illegalhandler>
  401423:	48 89 c6             	mov    %rax,%rsi
  401426:	bf 04 00 00 00       	mov    $0x4,%edi
  40142b:	e8 90 fc ff ff       	call   4010c0 <signal@plt>
  401430:	48 8b 05 09 2c 00 00 	mov    0x2c09(%rip),%rax        # 404040 <stdin@GLIBC_2.2.5>
  401437:	48 89 05 12 2c 00 00 	mov    %rax,0x2c12(%rip)        # 404050 <infile>
  40143e:	b8 00 00 00 00       	mov    $0x0,%eax
  401443:	e8 16 00 00 00       	call   40145e <getbuf>
  401448:	48 8d 05 f9 0c 00 00 	lea    0xcf9(%rip),%rax        # 402148 <_IO_stdin_used+0x148>
  40144f:	48 89 c7             	mov    %rax,%rdi
  401452:	e8 39 fc ff ff       	call   401090 <puts@plt>
  401457:	b8 00 00 00 00       	mov    $0x0,%eax
  40145c:	c9                   	leave
  40145d:	c3                   	ret

000000000040145e <getbuf>:
  40145e:	f3 0f 1e fa          	endbr64
  401462:	55                   	push   %rbp
  401463:	48 89 e5             	mov    %rsp,%rbp
  401466:	48 83 ec 20          	sub    $0x20,%rsp
  40146a:	48 8d 45 e0          	lea    -0x20(%rbp),%rax
  40146e:	48 89 c6             	mov    %rax,%rsi
  401471:	48 8d 05 f4 0c 00 00 	lea    0xcf4(%rip),%rax        # 40216c <_IO_stdin_used+0x16c>
  401478:	48 89 c7             	mov    %rax,%rdi
  40147b:	b8 00 00 00 00       	mov    $0x0,%eax
  401480:	e8 1b fc ff ff       	call   4010a0 <printf@plt>
  401485:	48 8d 45 e0          	lea    -0x20(%rbp),%rax
  401489:	48 89 c7             	mov    %rax,%rdi
  40148c:	e8 4d fe ff ff       	call   4012de <Gets>
  401491:	b8 01 00 00 00       	mov    $0x1,%eax
  401496:	c9                   	leave
  401497:	c3                   	ret

Disassembly of section .fini:

0000000000401498 <_fini>:
  401498:	f3 0f 1e fa          	endbr64
  40149c:	48 83 ec 08          	sub    $0x8,%rsp
  4014a0:	48 83 c4 08          	add    $0x8,%rsp
  4014a4:	c3                   	ret
