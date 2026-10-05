int main() {
    void *p = malloc(32);

    // Write arbitrary garbage into the payload
    memset(p, 0xFF, 32);

    // Now free it — allocator must still find the correct header
    free(p);

    printf("Payload independence test completed.\n");
}
