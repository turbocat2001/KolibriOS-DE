;
;*** �㭪樨 ��� ࠡ��� � ���ᥫ쭮� ��䨪�� ***
;

vox_offs_tree_table equ 4
vox_offs_data equ 12

;ᮧ����� ���ᥫ��� ���⥩
align 4
proc vox_brush_create uses eax ebx ecx edi, h_br:dword, buf_z:dword
	mov edi,[h_br]
	movzx ecx,byte[edi+3]
	add edi,4

	; *** ᮧ����� �����筮� ���� ***
	mov eax,[buf_z]
	mov buf2d_data,eax
	movzx eax,byte[edi-4] ;�ਭ� �����筮� ����
	mov buf2d_w,eax ;�ਭ� ����
	movzx eax,byte[edi-4+1] ;���� �����筮� ����
	mov buf2d_h,eax ;���� ����
	mov buf2d_size_lt,0 ;����� ᫥�� � �ࠢ� ��� ����
	mov buf2d_color,0 ;梥� 䮭� ����
	mov buf2d_bits,32 ;������⢮ ��� � 1-� �窥 ����ࠦ����

	; *** ᮧ����� ᫥����� ���⥩ ***
	cmp ecx,1
	jl .end_creat
	movzx ebx,byte[edi-4+2] ;���� �᭮����� �����筮� ����
	shr ebx,1
	cld
	@@:
		mov eax,edi
		add edi,BUF_STRUCT_SIZE
		stdcall vox_create_next_brush, eax, edi, ebx
		shl ebx,1
		loop @b
	.end_creat:
	ret
endp

;㤠����� ���ᥫ��� ���⥩
align 4
proc vox_brush_delete uses ecx edi, h_br:dword
	mov edi,[h_br]
	movzx ecx,byte[edi+3]
	add edi,4

	; *** 㤠����� ���⥩ ***
	cmp ecx,1
	jl .end_delete
	cld
	@@:
		add edi,BUF_STRUCT_SIZE
		stdcall buf_delete, edi
		loop @b
	.end_delete:
	ret
endp

;�㭪�� ��� ᮧ����� ���ᥫ� ᫥���饣� ���浪�
; buf_v1 - ���� � ��室�� ���ᥫ��
; buf_v2 - ���� � 㢥��祭� ���ᥫ��
; h - ���� �᭮����� ��室���� ���ᥫ� : 2
align 4
proc vox_create_next_brush uses eax ebx ecx edx edi, buf_v1:dword, buf_v2:dword, h:dword
	mov edi,[buf_v1]
	mov ebx,buf2d_h
	mov ecx,buf2d_w
	mov edi,[buf_v2]
	mov buf2d_h,ebx
	shl buf2d_h,1
	mov buf2d_w,ecx
	shl buf2d_w,1
	mov buf2d_color,0
	mov buf2d_bits,32

	stdcall buf_create, [buf_v2] ;ᮧ����� ���� ��㡨��
	shr ecx,1
	mov edx,[h]
	shl edx,1
	sub ebx,edx
	;ecx - �ਭ� ��室���� ���ᥫ� : 2
	;ebx - ���� ��室���� ���ᥫ� (��� �᭮�����)
	;edx - ���� �᭮����� ��室���� ���ᥫ�
	mov eax,[h]
	cmp eax,0
	je @f
		stdcall vox_add, [buf_v2], [buf_v1], ecx,0,0
		stdcall vox_add, [buf_v2], [buf_v1], ecx,ebx,0

		stdcall vox_add, [buf_v2], [buf_v1], 0,eax,eax
		push eax ;stdcall ...
		add eax,ebx
		stdcall vox_add, [buf_v2], [buf_v1], 0,eax ;,...
		sub eax,ebx
		shl ecx,1

		;ecx - �ਭ� ��室���� ���ᥫ�
		stdcall vox_add, [buf_v2], [buf_v1], ecx,eax,eax
		push eax ;stdcall ...,[h]
		add eax,ebx
		stdcall vox_add, [buf_v2], [buf_v1], ecx,eax;,[h]
		;sub eax,ebx
		shr ecx,1

		;ecx - �ਭ� ��室���� ���ᥫ� : 2
		stdcall vox_add, [buf_v2], [buf_v1], ecx,edx,edx
		add ebx,edx
		stdcall vox_add, [buf_v2], [buf_v1], ecx,ebx,edx

		jmp .end_0
	@@:
		;�᫨ h = 0, ⮣�� ����砥� ����� �� 2 �࠭�
		;� ⠪�� ��砥 ��� ����祭�� ��㡨�� ��६ �ਭ� / 2
		mov eax,ecx
		;2 ����� ���ᥫ�
		stdcall vox_add, [buf_v2], [buf_v1], 0,0,eax
		stdcall vox_add, [buf_v2], [buf_v1], 0,ebx,eax
		shl eax,1
		;2 業�ࠫ��� ��।��� ���ᥫ� (������ 業�ࠫ�� �� �뢮���)
		stdcall vox_add, [buf_v2], [buf_v1], ecx,0,eax
		stdcall vox_add, [buf_v2], [buf_v1], ecx,ebx,eax
		shr eax,1
		shl ecx,1
		;2 �ࠢ�� ���ᥫ�
		stdcall vox_add, [buf_v2], [buf_v1], ecx,0,eax
		stdcall vox_add, [buf_v2], [buf_v1], ecx,ebx,eax

	.end_0:


	ret
endp

;
align 4
proc vox_add, buf_v1:dword, buf_v2:dword, coord_x:dword, coord_y:dword, coord_z:dword
pushad
	mov ebx,[coord_x]
	mov eax,[coord_y]
	mov edi,[buf_v2]
	mov ecx,buf2d_h
	mov esi,buf2d_w
	imul ecx,esi
	add esi,ebx
	mov edx,buf2d_data
	cld
	;ecx - count pixels in voxel
	;edx - 㪠��⥫� �� ����� � ���ᥫ쭮� ����
	;edi - 㪠��⥫� �� ���ᥫ�� ����
	;esi - width voxel buffer add coord x
	.cycle:
		cmp dword[edx],0
		je @f
			;�஢��塞 ���� ��㡨��
			push eax ecx edi esi
			mov ecx,eax
			mov edi,[buf_v1]
			call get_pixel_32 ;stdcall buf_get_pixel, [buf_v1],ebx,ecx
			mov esi,[edx]
			add esi,[coord_z]
			cmp eax,esi
			jge .end_draw
			stdcall buf_set_pixel, [buf_v1],ebx,ecx,esi ;esi = new coord z
			.end_draw:
			pop esi edi ecx eax
		@@:
		add edx,4
		inc ebx
		cmp ebx,esi
		jl @f
			inc eax
			sub ebx,buf2d_w
		@@:
		loop .cycle
popad
	ret
endp

;description:
; �����蠥� �ਭ� ���ᥫ쭮�� ����ࠦ���� � 3-�� �࠭ﬨ
; �ਭ����� 㪠��⥫� �� ����� � ����⠡
align 4
proc buf_vox_obj_get_img_w_3g uses ecx, h_br:dword,k_scale:dword
	mov ecx,[h_br]

	movzx eax,byte[ecx]
	cmp dword[k_scale],1
	jl .end_c0
		mov ecx,[k_scale]
		shl eax,cl
	.end_c0:
	ret
endp

;description:
; �����蠥� ����� ���ᥫ쭮�� ����ࠦ���� � 3-�� �࠭ﬨ
; �ਭ����� 㪠��⥫� �� ����� � ����⠡
align 4
proc buf_vox_obj_get_img_h_3g uses ecx, h_br:dword,k_scale:dword
	mov ecx,[h_br]

	movzx eax,byte[ecx+1]
	cmp dword[k_scale],1
	jl .end_c0
		mov ecx,[k_scale]
		shl eax,cl
	.end_c0:
	ret
endp

;description:
; �㭪�� ������ ���ᥫ�� ��ꥪ� (����� 1 �࠭�)
;input:
; buf_i - ���� � ���஬ ������ (24 ���)
; buf_z - ���� ��㡨�� (32 ��� �� ��� ���ᥫ�� ������ ᮢ������ � buf_i)
align 4
proc buf_vox_obj_draw_1g, buf_i:dword, buf_z:dword, v_obj:dword, coord_x:dword,\
coord_y:dword, k_scale:dword
	cmp [k_scale],0
	jl .end_f
pushad
	mov edi,[buf_i]
	cmp buf2d_bits,24
	jne .error1
	mov edi,[buf_z]
	cmp buf2d_bits,32
	jne .error2

	mov ecx,[k_scale]
	mov ebx,[coord_x]
	mov edx,[coord_y]
	mov edi,[v_obj]
	add edi,vox_offs_data
	xor esi,esi
	stdcall draw_sub_vox_obj_1g, [buf_i],[buf_z],[v_obj]

	jmp .end_0
	.error1:
		stdcall print_err,sz_buf2d_vox_obj_draw_1g,txt_err_n24b
		jmp .end_0
	.error2:
		stdcall print_err,sz_buf2d_vox_obj_draw_1g,txt_err_n32b
	.end_0:
popad
	.end_f:
	ret
endp

;input:
; ebx - coord_x
; edx - coord_y
; esi - coord_z
; ecx - �஢��� ⥪�襣� 㧫�
; edi - 㪠��⥫� �� ����� ���ᥫ쭮�� ��ꥪ�
align 4
proc draw_sub_vox_obj_1g, buf_i:dword, buf_z:dword, v_obj:dword
	cmp byte[edi+3],0 ;ᬮ�ਬ ���� �� �����ॢ��
	je .sub_trees

		;���ᮢ�� ࠬ�� �᫨ ࠧ��� 㧫� = 1
		cmp ecx,0
		jne @f
			;�஢�ઠ ��㡨�� esi
			stdcall buf_get_pixel, [buf_z], ebx,edx, esi
			cmp eax,esi
			jge @f
				push ecx
				mov ecx,dword[edi]
				and ecx,0xffffff
				stdcall buf_set_pixel, [buf_i], ebx,edx, ecx
				stdcall buf_set_pixel, [buf_z], ebx,edx, esi
				pop ecx
		@@:

		;४��ᨢ�� ��ॡ�� �����ॢ쥢
		push edx
		;�室 ������ 㧫�
		dec ecx

		mov eax,1
		cmp ecx,1
		jl @f
			shl eax,cl
		@@:

		add edx,eax ;��४�஢�� ����� ��� ���ᥫ� ������� �஢��

		mov ah,byte[edi+3]
		add edi,4
		mov al,8
		.cycle:
			bt ax,8 ;����㥬 ⮫쪮 ah
			jnc .c_next
				push eax ebx edx esi
				stdcall vox_corect_coords_pl, [v_obj],1
				stdcall draw_sub_vox_obj_1g, [buf_i],[buf_z],[v_obj]
				pop esi edx ebx eax
			.c_next:
			shr ah,1
			dec al
			jnz .cycle
		;��室 �� 㧫�
		inc ecx
		pop edx
		jmp .end_f
	.sub_trees:
		cmp ecx,0
		jl .end_0 ;�� ��㥬 �祭� �����쪨� ���ᥫ�

			;��㥬 㧥�
			mov eax,[edi]
			and eax,0xffffff
			
			cmp ecx,1
			jl @f
				;������ ����� ⥪�饣� ����⠡�
				stdcall vox_draw_square_1g, [buf_i],[buf_z],eax
				jmp .end_0
			@@:
				;������ ⥪�饣� ����⠡�
				push ecx
				mov ecx,eax
				stdcall buf_get_pixel, [buf_z], ebx,edx
				cmp eax,esi
				jge .end_1
				stdcall buf_set_pixel, [buf_i], ebx,edx,ecx
				stdcall buf_set_pixel, [buf_z], ebx,edx,esi
				.end_1:
				pop ecx
		.end_0:
		add edi,4
	.end_f:
	ret
endp

;output:
; eax - ࠧ��蠥���
align 4
proc vox_draw_square_1g uses ecx edx edi, buf_i:dword, buf_z:dword, color:dword
locals
	img_size dd ?
	coord_y dd ?
endl
	mov edi,[buf_z]
	xor eax,eax
	inc eax
	shl eax,cl
	mov [img_size],eax
	mov [coord_y],eax
	.cycle_0:
	push ebx
	mov ecx,[img_size]
	cld
	.cycle_1:
		push ecx
		mov ecx,edx
		call get_pixel_32
		pop ecx
		cmp eax,esi
		jge @f
			stdcall buf_set_pixel, [buf_i], ebx,edx, [color]
			stdcall buf_set_pixel, edi, ebx,edx, esi
		@@:
		inc ebx
	loop .cycle_1
	pop ebx
	inc edx
	dec dword[coord_y]
	jnz .cycle_0
	ret
endp

;description:
; �㭪�� ������ ���ᥫ�� ��ꥪ� (����� 3 �࠭�)
;input:
; buf_i - ���� � ���஬ ������ (24 ���)
; buf_z - ���� ��㡨�� (32 ��� �� ��� ���ᥫ�� ������ ᮢ������ � buf_i)
; h_br - ����� � ����ࠦ���ﬨ ���ᥫ�� (32 ���)
; v_obj - ���ᥫ�� ��ꥪ�
; k_scale - ����. ��� ����⠡�஢���� ����ࠦ����
align 4
proc buf_vox_obj_draw_3g, buf_i:dword, buf_z:dword, h_br:dword, v_obj:dword,\
coord_x:dword, coord_y:dword, coord_z:dword, k_scale:dword
pushad
	mov edi,[v_obj]
	mov ecx,[k_scale]
	mov ebx,[coord_x]
	mov edx,[coord_y]
	add edi,vox_offs_data
	mov esi,[coord_z]
	stdcall vox_go_in_node, [buf_i], [buf_z], [h_br], [v_obj]
popad
	ret
endp

;description:
; �㭪�� ������ ���� ���ᥫ쭮�� ��ꥪ�
;input:
; buf_i - ���� � ���஬ ������ (24 ���)
; buf_z - ���� ��㡨�� (32 ��� �� ��� ���ᥫ�� ������ ᮢ������ � buf_i)
; h_br - ����� � ����ࠦ���ﬨ ���ᥫ�� (32 ���)
; v_obj - ���ᥫ�� ��ꥪ�
; k_scale - ����. ��� ����⠡�஢���� ����ࠦ����
align 4
proc buf_vox_obj_draw_3g_scaled, buf_i:dword, buf_z:dword, h_br:dword, v_obj:dword,\
coord_x:dword, coord_y:dword, coord_z:dword, k_scale:dword,\
s_c_x:dword, s_c_y:dword, s_c_z:dword, s_k_scale:dword,b_color:dword
pushad
locals
	p_node dd 0 ;த�⥫�᪨� 㧥�
endl
	mov edi,[v_obj]
	add edi,vox_offs_data

	mov ecx,[k_scale]
	mov ebx,[coord_x]

	;��⮢�� ࠬ��
	mov eax,[h_br]

	movzx edx,byte[eax]
	movzx esi,byte[eax+1]
	cmp ecx,1
	jl .end_c0
		shl edx,cl
		shl esi,cl
	.end_c0:
	;stdcall buf_rect_by_size, [buf_i], ebx,[coord_y],edx,esi, [b_color]

	;���⨪��쭠� �����
	add ebx,edx
	shr edx,cl
	stdcall buf_rect_by_size, [buf_i], ebx,[coord_y],edx,esi, [b_color]
	mov ecx,[s_k_scale]
	shr esi,cl
	xor eax,eax
	inc eax
	shl eax,cl
	dec eax
	sub eax,[s_c_z] ;���祭�� �� �� z �������� � ���� �����
	imul eax,esi
	add eax,[coord_y]
	stdcall buf_filled_rect_by_size, [buf_i], ebx,eax,edx,esi, [b_color]
	mov ebx,[coord_y]
	shl esi,cl
	add ebx,esi
	stdcall buf_vox_obj_get_img_w_3g, [h_br],[k_scale]
	shr eax,1
	mov esi,[h_br]
	movzx esi,byte[esi+1]
	;����㭮�
	stdcall draw_polz_hor, [buf_i], [coord_x],ebx,eax,esi, [s_c_x], [s_k_scale], [b_color]
	mov edx,[coord_x]
	add edx,eax
	;����㭮�
	stdcall draw_polz_hor, [buf_i], edx,ebx,eax,esi, [s_c_y], [s_k_scale], [b_color]
;---

	mov esi,[s_k_scale]
	cmp esi,1
	jl .end_2

	; *** (1) ***
	.found:
	stdcall vox_obj_get_node_position, [v_obj],[s_c_x],[s_c_y],[s_c_z],esi
	movzx bx,byte[edi+3]
	mov [p_node],edi
	add edi,4
	cmp eax,0
	je .end_1
	mov ecx,eax
	cld
	@@: ;横� ��� �ய�᪠ �।���� �����ॢ쥢 � 㧫�
		bt bx,0 ;�஢��塞 ���� �� ���୨� 㧫�
		jnc .end_0
			xor eax,eax
			stdcall vox_obj_rec0 ;� eax �������� �᫮ ���୨� 㧫��, � ������ ��⢨
		.end_0:
		shr bx,1
		loop @b
	.end_1:
	bt bx,0
	jnc .end_2 ;�᫨ �����ॢ� �� �������
	dec esi
	cmp esi,0
	jg .found

	; *** (2) ***
	;�ᮢ���� ��� ��ꥪ�
	mov ecx,[k_scale]
	mov ebx,[coord_x]
	mov edx,[coord_y]
	mov esi,[coord_z]
	stdcall vox_go_in_node, [buf_i], [buf_z], [h_br], [v_obj]
	.end_2:

popad
	ret
endp

;input:
; h_br - ����� � ����ࠦ���ﬨ ���ᥫ�� (32 ���)
; ebx - coord_x
; edx - coord_y
; esi - coord_z
; ecx - �஢��� ⥪�襣� 㧫�
; edi - 㪠��⥫� �� ����� ���ᥫ쭮�� ��ꥪ�
align 4
proc vox_go_in_node, buf_i:dword, buf_z:dword, h_br:dword, v_obj:dword
	cmp byte[edi+3],0 ;ᬮ�ਬ ���� �� �����ॢ��
	je .sub_trees
		;४��ᨢ�� ��ॡ�� �����ॢ쥢
		push eax edx

		;���ᮢ�� ࠬ�� �᫨ ࠧ��� 㧫� = 1
		cmp ecx,0
		jne .end_2
			push eax
				stdcall vox_get_sub_brush,[h_br],0 ;��।��塞 ����� ��� �ᮢ����
				cmp eax,0 ;�᫨ ����� �� �������
				je @f
					stdcall draw_vox, [buf_i], [buf_z], eax, ebx,edx,esi, [edi]
				@@:
			pop eax
		.end_2:

		;�室 ������ 㧫�
		dec ecx
;---
		push ebx
			;mov eax,(h-h_osn/2)
			mov ebx,[h_br]
			movzx eax,byte[ebx+1]
			cmp byte[ebx+2],0
			je @f
				;�᫨ ����� � 3-�� �࠭ﬨ
				movzx ebx,byte[ebx+2]
				shr ebx,1
				sub eax,ebx
				jmp .end_0
			@@:
				;�᫨ ����� � 2-�� �࠭ﬨ
				movzx ebx,byte[ebx]
				shr ebx,1
			.end_0:
		cmp ecx,1
		jl @f
			shl eax,cl
			shl ebx,cl
		@@:
		add esi,ebx
		pop ebx
		add edx,eax ;��४�஢�� ����� ��� ���ᥫ� ������� �஢��
;---
		mov ah,byte[edi+3]
		add edi,4
		mov al,8
		.cycle:
			bt ax,8 ;����㥬 ⮫쪮 ah
			jnc .c_next
				push ebx edx esi
				stdcall vox_corect_coords, [h_br], [v_obj]
				stdcall vox_go_in_node, [buf_i], [buf_z], [h_br], [v_obj]
				pop esi edx ebx
			.c_next:
			shr ah,1
			dec al
			jnz .cycle

		;��室 �� 㧫�
		inc ecx
		pop edx eax

		jmp .end_f
	.sub_trees:
		;��㥬 㧥�
		push eax
			stdcall vox_get_sub_brush,[h_br],ecx ;��।��塞 ����� ��� �ᮢ����
			cmp eax,0 ;�᫨ ����� �� �������
			je @f
				stdcall draw_vox, [buf_i], [buf_z], eax, ebx,edx,esi, [edi]
			@@:
		pop eax

		add edi,4
	.end_f:
	ret
endp

;description:
; �㭪�� ������ ������� ���ᥫ
;input:
; buf_i - ���� � ���஬ ������ (24 ���)
; buf_z - ���� ��㡨�� (32 ��� �� ��� ���ᥫ�� ������ ᮢ������ � buf_i)
; buf_v - ���� � ����ࠦ����� ���ᥫ� (32 ���)
; v_color - 梥�
align 4
proc draw_vox, buf_i:dword, buf_z:dword, buf_v:dword,\
coord_x:dword, coord_y:dword, coord_z:dword, v_color:dword
pushad
	mov eax,[coord_x]
	mov ebx,[coord_y]
	mov edi,[buf_v]
	mov ecx,buf2d_h
	mov esi,buf2d_w
	imul ecx,esi
	add esi,eax
	mov edx,buf2d_data
	cld
	;ecx - count pixels in voxel
	;edx - 㪠��⥫� �� ����� � ���ᥫ쭮� ����
	;edi - 㪠��⥫� �� ���ᥫ�� ����
	;esi - width voxel buffer add coord x
	.cycle:
		cmp dword[edx],0
		je @f
			;�஢��塞 ���� ��㡨��
			push eax
			stdcall buf_get_pixel, [buf_z],eax,ebx
			sub eax,[coord_z]
			cmp eax,[edx]
			jl .dr_pixel
				pop eax
				jmp @f
			.dr_pixel:
				;��㥬 ���
				pop eax
				stdcall buf_set_pixel, [buf_i],eax,ebx,[v_color]
				push ecx
				mov ecx,[coord_z]
				add ecx,[edx]
				stdcall buf_set_pixel, [buf_z],eax,ebx,ecx
				pop ecx
		@@:
		add edx,4
		inc eax
		cmp eax,esi
		jl @f
			inc ebx
			sub eax,buf2d_w
		@@:
		loop .cycle
popad
	ret
endp

;description:
;�㭪�� ��� ��४�஢�� ���न���
;���ࠢ����� �ᥩ ���न��� � ���ᥫ�:
;*z
;|
;+
;  * y
; /
;+
; \
;  * x
;input:
;  al - ����� 㧫� � ��ॢ� (�� 1 �� 8)
; ebx - ���न��� x
; edx - ���न��� y
; esi - ���न��� z
; ecx - �஢��� ⥪�襣� 㧫�
;output:
; ebx - ����� ���न��� x
; edx - ����� ���न��� y
; esi - ����� ���न��� z
align 4
proc vox_corect_coords, h_br:dword, v_obj:dword
locals
	osn_w_2 dd ? ;�ਭ� �᭮����� �����筮�� ���ᥫ� : 2
	vox_h dd ? ;���� �����筮�� ���ᥫ�
endl
	cmp ecx,0
	jl .end_f ;��� �᪮७�� ���ᮢ��

	push eax edi
	and eax,15 ;�뤥�塞 ����� 㧫� � ��ॢ�
	mov edi,[v_obj]
	add edi,vox_offs_tree_table
	add edi,8
	sub edi,eax

	push ebx ecx
		mov ebx,[h_br]

		movzx ecx,byte[ebx]
		shr ecx,1
		mov dword[osn_w_2],ecx

		movzx ecx,byte[ebx+2]
		movzx ebx,byte[ebx+1]
		sub ebx,ecx
		mov dword[vox_h],ebx
		shr ecx,1
		mov eax,ecx ;eax - ���� �᭮����� �����筮�� ���ᥫ� : 2
	pop ecx ebx

	cmp ecx,1
	jl @f ;�� ��������� ��横�������
		shl eax,cl
		shl dword[osn_w_2],cl
		shl dword[vox_h],cl
	@@:

;	add esi,eax ;���塞 ��㡨�� ��� ���� z (��������� ��� ���न���� y)
	bt word[edi],0 ;test voxel coord x
	jnc @f
		add ebx,[osn_w_2]
		cmp eax,0
		jne .end_0
			add esi,[osn_w_2] ;���塞 ��㡨�� ��� ���� z
			jmp @f
		.end_0:
		add edx,eax
		add esi,eax ;���塞 ��㡨�� ��� ���� z
	@@:
	bt word[edi],1 ;test voxel coord y
	jnc @f
		add ebx,[osn_w_2]
		cmp eax,0
		jne .end_1
			sub esi,[osn_w_2] ;���塞 ��㡨�� ��� ���� z
			jmp @f
		.end_1:
		sub edx,eax
		sub esi,eax ;���塞 ��㡨�� ��� ���� z
	@@:
	bt word[edi],2 ;test voxel coord z
	jnc @f
		sub edx,[vox_h]
	@@:
	pop edi eax
	.end_f:
	ret
endp

;��������� �� h_br 㪠��⥫� �� ���� � ����ࠦ����� ���ᥫ�, 㪠������� ���浪� n
align 4
proc vox_get_sub_brush uses ebx ecx, h_br:dword, n:dword
	xor eax,eax
	mov ebx,[n]
	cmp ebx,0
	jl @f
	mov ecx,[h_br]
	cmp bl,byte[ecx+3]
	jg @f
		add ecx,4
		imul ebx,BUF_STRUCT_SIZE
		mov eax,ebx
		add eax,ecx
	@@:
	ret
endp

;description:
; �㭪�� ������ �१ ���ᥫ쭮�� ��쥪�
;input:
; v_size - ࠧ��� ������ � ���ᥫ��
; k_scale - �⥯��� ��⠫���樨 ����ࠦ����
; n_plane - ����� ���᪮�� �筨� (� �।���� �� 0 �� 2^k_scale - 1)
; b_color - 梥� �࠭���
align 4
proc buf_vox_obj_draw_pl, buf_i:dword, v_obj:dword, coord_x:dword,\
coord_y:dword, v_size:dword, k_scale:dword, n_plane:dword, b_color:dword
	cmp [k_scale],0
	jl .end_f
pushad
	mov eax,[v_size]
	mov ecx,[k_scale]
	mov ebx,eax
	cmp ecx,1
	jl @f
		shl ebx,cl
	@@:
	;ebx - ����� ࠧ��� ����ࠦ����
	stdcall buf_rect_by_size, [buf_i], [coord_x],[coord_y],ebx,ebx, [b_color] ;ࠬ�� �� ��㭮�
	mov edx,ebx
	add ebx,[coord_y]
	stdcall draw_polz_hor, [buf_i], [coord_x],ebx,edx,eax, [n_plane], [k_scale], [b_color] ;����㭮�, �����뢠�騩 ����� �祭��

	;�ᮢ���� �祪 ��� �⪨
	push ecx
	mov edi,1
	cmp ecx,1
	jl @f
		shl edi,cl
	@@:
	dec edi
	cmp edi,1
	jl .end_0
	mov ecx,edi
	imul ecx,edi
	mov ebx,[coord_x]
	mov edx,[coord_y]
	add edx,eax
	xor esi,esi
	cld
	@@:
		add ebx,eax
		inc esi
		stdcall buf_set_pixel, [buf_i], ebx,edx, [b_color]
		cmp esi,edi
		jl .end_1
			;���室 �祪 �� ����� ��ப�
			xor esi,esi
			mov ebx,[coord_x]
			add edx,eax
		.end_1:
		loop @b
	.end_0:
	pop ecx

	;eax - ࠧ��� ������ ������
	;edi - 㪠��⥫� �� ��㥬� ����� �� ��ꥪ�
	mov ebx,[coord_x]
	mov edx,[coord_y]
	mov edi,[v_obj]
	add edi,vox_offs_data
	xor esi,esi
	push eax
	mov eax,1
	shl eax,cl
	dec eax
	sub eax,[n_plane]
	stdcall draw_sub_vox_obj_pl, [buf_i],[v_obj],eax
popad
	.end_f:
	ret
endp

;description:
; �㭪�� ������ �१ ��� ���ᥫ쭮�� ��쥪�
;input:
; s_c_x, s_c_y, s_c_z, s_k_scale - ��ࠬ���� ��।����騥 ���� ���ᥫ쭮�� ��ꥪ�, ����� �㤥� �ᮢ�����
align 4
proc buf_vox_obj_draw_pl_scaled, buf_i:dword, v_obj:dword, coord_x:dword,\
coord_y:dword, v_size:dword, k_scale:dword, n_plane:dword, b_color:dword,\
s_c_x:dword, s_c_y:dword, s_c_z:dword, s_k_scale:dword
	cmp [k_scale],0
	jl .end_f
pushad
locals
	p_node dd 0 ;த�⥫�᪨� 㧥�
endl
	mov eax,[v_size]
	mov ecx,[k_scale]
	mov ebx,eax
	cmp ecx,1
	jl @f
		shl ebx,cl
	@@:
	;ebx - ����� ࠧ��� ����ࠦ����
	stdcall buf_rect_by_size, [buf_i], [coord_x],[coord_y],ebx,ebx, [b_color] ;ࠬ�� �� ��㭮�
	mov edx,ebx
	add ebx,[coord_y]
	stdcall draw_polz_hor, [buf_i], [coord_x],ebx,edx,eax, [n_plane], [k_scale], [b_color] ;����㭮�, �����뢠�騩 ����� �祭��

	;�ᮢ���� �祪 ��� �⪨
	push ecx
	mov edi,1
	cmp ecx,1
	jl @f
		shl edi,cl
	@@:
	dec edi
	cmp edi,1
	jl .end_3
	mov ecx,edi
	imul ecx,edi
	mov ebx,[coord_x]
	mov edx,[coord_y]
	add edx,eax
	xor esi,esi
	cld
	@@:
		add ebx,eax
		inc esi
		stdcall buf_set_pixel, [buf_i], ebx,edx, [b_color]
		cmp esi,edi
		jl .end_4
			;���室 �祪 �� ����� ��ப�
			xor esi,esi
			mov ebx,[coord_x]
			add edx,eax
		.end_4:
		loop @b
	.end_3:
	pop ecx

	mov esi,[s_k_scale]
	cmp esi,1
	jl .end_2
	mov edi,[v_obj]
	add edi,vox_offs_data

	; *** (1) ***
	.found:
	stdcall vox_obj_get_node_position, [v_obj],[s_c_x],[s_c_y],[s_c_z],esi
	movzx bx,byte[edi+3]
	mov [p_node],edi
	add edi,4
	cmp eax,0
	je .end_1
	mov ecx,eax
	cld
	@@: ;横� ��� �ய�᪠ �।���� �����ॢ쥢 � 㧫�
		bt bx,0 ;�஢��塞 ���� �� ���୨� 㧫�
		jnc .end_0
			xor eax,eax
			stdcall vox_obj_rec0 ;� eax �������� �᫮ ���୨� 㧫��, � ������ ��⢨
		.end_0:
		shr bx,1
		loop @b
	.end_1:
	bt bx,0
	jnc .end_2 ;�᫨ �����ॢ� �� �������
	dec esi
	cmp esi,0
	jg .found

	mov eax,[v_size]
	;eax - ࠧ��� ������ ������
	;edi - 㪠��⥫� �� ��㥬� ����� �� ��ꥪ�
	mov ecx,[k_scale]
	mov ebx,[coord_x]
	mov edx,[coord_y]
	xor esi,esi
	push eax
	mov eax,1
	shl eax,cl
	dec eax
	sub eax,[n_plane]
	stdcall draw_sub_vox_obj_pl, [buf_i],[v_obj], eax

	.end_2:
popad
	.end_f:
	ret
endp

;description:
; ��।������ ����樨 㧫� � ��ॢ� (�� 0 �� 7)
align 4
proc vox_obj_get_node_position uses ebx ecx edi, v_obj:dword,\
coord_x:dword,coord_y:dword,coord_z:dword,k_scale:dword
	mov ecx,[k_scale]
	dec ecx
	mov eax,[coord_x]
	mov ebx,[coord_y]
	mov edi,[coord_z]
	cmp ecx,1
	jl .end_0
		shr eax,cl
		shr ebx,cl
		shr edi,cl
	.end_0:
	and eax,1
	bt ebx,0
	jnc @f
		bts eax,1
	@@:
	bt edi,0
	jnc @f
		bts eax,2
	@@:

	mov edi,[v_obj]
	add edi,vox_offs_tree_table
	@@:
		cmp al,byte[edi]
		je @f
		inc edi
		jmp @b
	@@:
	sub edi,[v_obj]
	sub edi,vox_offs_tree_table
	mov eax,edi
	
	ret
endp

;input:
; edi - 㪠��⥫� �� ����� ���ᥫ쭮�� ��ꥪ�
;output:
; eax - eax + �᫮ 㧫�� � ������ ����. ��ꥪ�
; edi - 㪠��⥫� �� ᬥ饭�� ����� ����. ��ꥪ�
align 4
proc vox_obj_rec0
	inc eax
	cmp byte[edi+3],0 ;ᬮ�ਬ ���� �� �����ॢ��
	je .sub_trees

		;४��ᨢ�� ��ॡ�� �����ॢ쥢
		push ebx ecx
		mov bh,byte[edi+3]
		add edi,4
		mov bl,8
		.cycle:
			bt bx,8 ;����㥬 ⮫쪮 bh
			jnc .c_next
				stdcall vox_obj_rec0
			.c_next:
			shr bh,1
			dec bl
			jnz .cycle
		pop ecx ebx

		jmp .end_f
	.sub_trees:
		add edi,4
	.end_f:
	ret
endp

;description:
; �㭪�� ������ ��ਧ��⠫��� ������ � ����㭪��
align 4
proc draw_polz_hor uses eax ebx ecx, buf:dword, coord_x:dword, coord_y:dword,\
size_x:dword, size_y:dword, pos:dword, k_scale:dword, color:dword
	mov ebx,[size_x]
	stdcall buf_rect_by_size, [buf], [coord_x],[coord_y],ebx,[size_y], [color]
	mov ecx,[k_scale]
	shr ebx,cl
	mov eax,[pos]
	imul eax,ebx
	add eax,[coord_x]
	stdcall buf_filled_rect_by_size, [buf], eax,[coord_y],ebx,[size_y], [color]
	ret
endp

;input:
; ebx - coord_x
; edx - coord_y
; esi - coord_z
; ecx - �஢��� ⥪�襣� 㧫�
; edi - 㪠��⥫� �� ����� ���ᥫ쭮�� ��ꥪ�
align 4
proc draw_sub_vox_obj_pl, buf_i:dword, v_obj:dword, clip_z:dword,\
v_size:dword
	cmp byte[edi+3],0 ;ᬮ�ਬ ���� �� �����ॢ��
	je .sub_trees

		;���ᮢ�� ࠬ�� �᫨ ࠧ��� 㧫� = 1
		cmp ecx,0
		jne @f
			;�஢�ઠ ��㡨�� esi
			;clip_z=n_plane
			stdcall vox_is_clip, [clip_z];,[v_size]
			cmp eax,0
			je @f
				push ecx
				mov ecx,dword[edi]
				and ecx,0xffffff
				stdcall buf_rect_by_size, [buf_i], ebx,edx, [v_size],[v_size],ecx
				pop ecx
		@@:

		;४��ᨢ�� ��ॡ�� �����ॢ쥢
		push edx
		;�室 ������ 㧫�
		dec ecx

		mov eax,[v_size]
		cmp ecx,1
		jl @f
			shl eax,cl
		@@:

		add edx,eax ;��४�஢�� ����� ��� ���ᥫ� ������� �஢��

		mov ah,byte[edi+3]
		add edi,4
		mov al,8
		.cycle:
			bt ax,8 ;����㥬 ⮫쪮 ah
			jnc .c_next
				push eax ebx edx esi
				stdcall vox_corect_coords_pl, [v_obj],[v_size]
				stdcall draw_sub_vox_obj_pl, [buf_i],[v_obj],[clip_z],[v_size]
				pop esi edx ebx eax
			.c_next:
			shr ah,1
			dec al
			jnz .cycle
		;��室 �� 㧫�
		inc ecx
		pop edx
		jmp .end_f
	.sub_trees:
		cmp ecx,0
		jl .end_0 ;�� ��㥬 �祭� �����쪨� ���ᥫ�

			;�஢�ઠ ��㡨�� esi
			;clip_z=n_plane
			stdcall vox_is_clip, [clip_z]
			cmp eax,0
			je .end_0

			;��㥬 㧥�
			mov eax,[edi]
			and eax,0xffffff
			push eax ;梥� 㧫�

			mov eax,[v_size]
			cmp ecx,1
			jl @f
				;������ ����� ⥪�饣� ����⠡�
				shl eax,cl ;ࠧ��� 㧫�
				stdcall buf_filled_rect_by_size, [buf_i], ebx,edx, eax,eax
				push ebx edx esi
				mov esi,eax
				inc ebx
				inc edx
				sub esi,2
				mov eax,[buf_i]
				push dword 128
				push dword[eax+16] ;+16 - b_color
				stdcall combine_colors_3,[edi]
				stdcall buf_rect_by_size, [buf_i], ebx,edx, esi,esi,eax
				pop esi edx ebx
				jmp .end_0
			@@:
				;������ ⥪�饣� ����⠡�
				stdcall buf_filled_rect_by_size, [buf_i], ebx,edx, eax,eax
		.end_0:
		add edi,4
	.end_f:
	ret
endp

;description:
; �ᯮ����⥫쭠� �㭪�� ��� �஢�ન ��㡨�� esi
;input:
; ecx - �஢��� ⥪�襣� 㧫�
; esi - coord z
; clip_z - n_plane
;output:
; eax - 0 if no draw, 1 if draw
align 4
proc vox_is_clip uses ebx edi, clip_z:dword
	xor eax,eax
	mov ebx,[clip_z]
	mov edi,1
	cmp ecx,1
	jl @f
		shl edi,cl
	@@:
	;edi = 2^ecx
	add edi,esi
	cmp edi,ebx ;if (esi+2^ecx <= n_plane) no draw
	jle @f
	inc ebx
	cmp esi,ebx ;if (esi >= (n_plane+1)) no draw
	jge @f
		inc eax
	@@:
	ret
endp

;�㭪�� ��� ��४�஢�� ���न���
;���ࠢ����� �ᥩ ���न��� � ���ᥫ�:
;*z
;|
;+-* x
;input:
;  al - ����� 㧫� � ��ॢ� (�� 1 �� 8)
; ebx - ���न��� x
; edx - ���न��� y
; esi - ���न��� z
; ecx - �஢��� ⥪�襣� 㧫�
;output:
; ebx - ����� ���न��� x
; edx - ����� ���न��� y
; esi - ����� ���न��� z
align 4
proc vox_corect_coords_pl, v_obj:dword, v_size:dword
	cmp ecx,0
	jl .end_f ;��� �᪮७�� ���ᮢ��

	push eax edi
	and eax,15 ;�뤥�塞 ����� 㧫� � ��ॢ�
	mov edi,[v_obj]
	add edi,vox_offs_tree_table
	add edi,8
	sub edi,eax

	mov eax,[v_size]
	cmp ecx,1
	jl @f
		shl eax,cl
	@@:

	bt word[edi],0 ;test voxel coord x
	jnc @f
		add ebx,eax
	@@:
	bt word[edi],2 ;test voxel coord z
	jnc @f
		sub edx,eax
	@@:
	bt word[edi],1 ;test voxel coord y
	jc @f
		mov eax,1
		cmp ecx,1
		jl .end_0
			shl eax,cl
		.end_0:
		add esi,eax ;���塞 ��㡨�� ��� ���� z
	@@:
	pop edi eax
	.end_f:
	ret
endp

;description:
; �㭪�� ������ ⥭�
;input:
; buf_i - ���� � ���஬ ������ (24 ���)
; buf_z - ���� ��㡨�� (32 ��� �� ��� ���ᥫ�� ������ ᮢ������ � buf_i)
; h_br - ����� � ����ࠦ���ﬨ ���ᥫ�� (32 ���)
; k_scale - ����. ��� ����⠡�஢���� ����ࠦ����
align 4
proc buf_vox_obj_draw_3g_shadows, buf_i:dword, buf_z:dword, h_br:dword, \
coord_x:dword, coord_y:dword, color:dword, k_scale:dword, prop:dword
locals
	correct_z dd 0 ;��४�஢�� ��� ���� ��㡨��
endl
pushad
	mov eax,[k_scale]
	add eax,[prop]
	mov dword[correct_z],8
	sub [correct_z],eax
	mov ebx,[coord_x]
	;correct_z = 8-k_scale-prop

	stdcall buf_vox_obj_get_img_w_3g, [h_br],[k_scale]
	mov edx,eax ;edx - �ਭ� ����ࠦ����
	stdcall buf_vox_obj_get_img_h_3g, [h_br],[k_scale]
	mov esi,eax

	mov edi,[coord_y]
	mov ecx,edx
	add edx,ebx ;�ਭ� + ����� ᫥��
	imul ecx,esi
	cld
	.cycle_0:
		stdcall buf_get_pixel, [buf_z],ebx,edi
		cmp eax,0
		je @f
			stdcall vox_correct_z, [correct_z]
			push eax
			stdcall buf_get_pixel, [buf_i],ebx,edi
			stdcall combine_colors_3,eax,[color] ;,eax
			stdcall buf_set_pixel, [buf_i],ebx,edi,eax
		@@:
		inc ebx
		cmp ebx,edx
		jl @f
			mov ebx,[coord_x]
			inc edi
		@@:
		loop .cycle_0

popad
	ret
endp

;output:
; eax - scaled coord z
align 4
proc vox_correct_z uses ecx, correct_z:dword
	mov ecx,[correct_z]
	cmp ecx,0
	je .end_f
	jl .end_0
		shl eax,cl
		jmp .end_f
	.end_0:
		neg ecx
		inc ecx
		shr eax,cl
	.end_f:
	ret
endp

;output:
; eax - color
align 4
proc combine_colors_3 uses ebx ecx edx edi esi, col_0:dword, col_1:dword, alpha:dword

	mov ebx,[col_0]
	mov ecx,[col_1]
	movzx di,byte[alpha] ;pro
	mov si,0x00ff ;---get transparent---
	sub si,di ;256-pro

	;---blye---
	movzx ax,bl
	imul ax,si
	movzx dx,cl
	imul dx,di
	add ax,dx
	mov cl,ah
	;---green---
	movzx ax,bh
	imul ax,si
	movzx dx,ch
	imul dx,di
	add ax,dx
	mov ch,ah
	shr ebx,16
	ror ecx,16
	;---red---
	movzx ax,bl
	imul ax,si
	movzx dx,cl
	imul dx,di
	add ax,dx

	shl eax,8
	ror ecx,16
	mov ax,cx
	and eax,0xffffff

	ret
endp