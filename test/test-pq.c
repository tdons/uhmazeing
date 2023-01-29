#include <stdbool.h>
#include <stdio.h>

#include "../src/pq.c"

bool test()
{
	struct mz_mpq_t *pq = mz_mpq_create_monotone(3);
	fprintf(stderr, "mz_mpq_create_monotone done\n");
	if (!mz_mpq_empty(pq)) {
		return false;
	}
	fprintf(stderr, "mz_mpq_empty\n");
	mz_mpq_insert(pq, 2, 3);
	fprintf(stderr, "insert 3\n");
	mz_mpq_insert(pq, 1, 2);
	fprintf(stderr, "insert 2\n");
	mz_mpq_insert(pq, 0, 1);
	fprintf(stderr, "insert 1\n");
	if (mz_mpq_empty(pq)) {
		return false;
	}
	fprintf(stderr, "!empty\n");
	if (mz_mpq_pop(pq) != 1) {
		return false;
	}
	fprintf(stderr, "pop 1\n");
	if (mz_mpq_pop(pq) != 2) {
		return false;
	}
	fprintf(stderr, "pop 2\n");
	if (mz_mpq_pop(pq) != 3) {
		return false;
	}
	fprintf(stderr, "pop 3\n");
	if (!mz_mpq_empty(pq)) {
		return false;
	}
	fprintf(stderr, "empty\n");
	mz_mpq_destroy(pq);
	fprintf(stderr, "cleaned up\n");
	return true;
}

int main()
{
	if (!test()) {
		return 1;
	}
	return 0;
}
