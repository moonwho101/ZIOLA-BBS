#include <stdio.h>
#include <stdlib.h>

main() {

	FILE *fp;

	struct last_read {
		long int mes_pnt[50];
		long int base_pnt[50];
		unsigned char scan[50];
	} lr_pnt;

	struct user_idx {
		unsigned char name[41];
		long int left;
		long int right;
	} user_tree;

	size_t rec_size2 = sizeof(struct user_idx);

	long int number, count, i;

	if ((fp = fopen("c:\\bz\\users\\users.idx", "rb+")) == NULL) {
		printf("Cannot open users.idx\n");
		exit(1);
	}

	fseek(fp, 0L, 0);
	fread(&user_tree, rec_size2, 1, fp);
	number = user_tree.left;

	fclose(fp);

	if ((fp = fopen("users.pnt", "wb+")) == NULL) {
		printf("cannot open users.idx\n");
		exit(1);
	}

	for (count = 0; count < 50; count++) {
		lr_pnt.mes_pnt[count] = 1L;
		lr_pnt.base_pnt[count] = 1L;
	}
	for (count = 0; count < 50; count++) {

		if (count < 5)
			lr_pnt.scan[count] = 'Y';
		else
			lr_pnt.scan[count] = 'N';
	}

	for (i = 0; i < number; i++) {

		fseek(fp, i * sizeof(struct last_read), 0);
		fwrite(&lr_pnt, sizeof(struct last_read), 1, fp);

		printf("%d %d\n", i, (int)number);
	}

	fclose(fp);
}
/*
      fseek(fp,record*rec_size,0);
      fwrite (&user_tree, rec_size,1,fp);
*/
