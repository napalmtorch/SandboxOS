#include <core/system/tests.h>
#include <core/kernel.h>

namespace os
{
    namespace sys
    {
        namespace tests
        {
            bool test_heap(int iterations)
            {
                uint32_t used_before = kernel::heap_large.calc_used() + kernel::heap_small.calc_used();
                uint32_t used_after  = 0;
                for (size_t i = 0; i < iterations; i++)
                {
                    void* test1 = tmalloc(128, ALLOCTYPE_STRING);
                    void* test2 = tmalloc(32 * KB, ALLOCTYPE_ARRAY);
                    void* test3 = tmalloc(256 * KB, ALLOCTYPE_ARRAY);
                    void* test4 = tmalloc(8192, ALLOCTYPE_OBJ);
                    void* test5 = tmalloc(6, ALLOCTYPE_OBJ);
                    void* test6 = tmalloc(0x10000, ALLOCTYPE_STRING);
                    if (!free(test4)) { perror("Failed to free pointer at 0x%8x", test4); }
                    if (!free(test2)) { perror("Failed to free pointer at 0x%8x", test2); }
                    if (!free(test3)) { perror("Failed to free pointer at 0x%8x", test3); }
                    if (!free(test1)) { perror("Failed to free pointer at 0x%8x", test1); }
                    if (!free(test6)) { perror("Failed to free pointer at 0x%8x", test6); }
                    if (!free(test5)) { perror("Failed to free pointer at 0x%8x", test5); }

                    uint32_t mc1 = kernel::heap_large.merge();
                    uint32_t mc2 = kernel::heap_small.merge();
                    if (mc1 + mc2 == 0) { perror("Expected heap merge count to be greater than 0"); return false; }
                    //else { printf("%s Merged %u free heap entries\n", DEBUG_INFO, mc1 + mc2); }
                }

                used_after = kernel::heap_large.calc_used() + kernel::heap_small.calc_used();
                if (used_before != used_after) { perror("Memory leak while testing stream - PRE:%u bytes POST:%u bytes", used_before, used_after); return false; }
                printf("%s Heap test passed\n", DEBUG_OK);
                return true;
            }

            bool test_stream(int iterations)
            {
                const char* str = "A test string blah blah blah blah";

                uint32_t used_before = kernel::heap_large.calc_used() + kernel::heap_small.calc_used();
                uint32_t used_after  = 0;
                for (size_t i = 0; i < iterations; i++)
                {
                    std::stream<std::string> stream = std::stream<std::string>(256);
                    for (size_t j = 0; j < stream.length(); j++) 
                    { 
                        stream[j] = std::string((char*)str); 
                        if (strcmp(stream[j].c_str(), str)) { perror("Invalid value at stream index %u", j); return false; }
                        stream[j].dispose();
                    }
                    stream.dispose();
                    uint32_t mc1 = kernel::heap_large.merge();
                    uint32_t mc2 = kernel::heap_small.merge();
                    if (mc1 + mc2 == 0) { perror("Expected heap merge count to be greater than 0"); return false; }
                    //else { printf("%s Merged %u free heap entries\n", DEBUG_INFO, mc1 + mc2); }
                }

                used_after = kernel::heap_large.calc_used() + kernel::heap_small.calc_used();
                if (used_before != used_after) { perror("Memory leak while testing stream - PRE:%u bytes POST:%u bytes", used_before, used_after); return false; }
                printf("%s Stream test passed\n", DEBUG_OK);
                return true;
            }

            bool test_arraylist(int iterations)
            {
                const char* str = "A test string blah blah blah blah";

                uint32_t used_before = kernel::heap_large.calc_used() + kernel::heap_small.calc_used();
                uint32_t used_after  = 0;
                for (size_t i = 0; i < iterations; i++)
                {
                    std::arraylist<std::string> list = std::arraylist<std::string>();
                    for (size_t j = 0; j < 256; j++) { list.add(std::string((char*)str)); }
                    for (size_t j = 0; j < list.length(); j++) 
                    { 
                        if (strcmp(list[j].c_str(), str)) { perror("Invalid value at arraylist index %u", j); return false; }
                        list[j].dispose();
                    }
                    list.dispose();
                    uint32_t mc1 = kernel::heap_large.merge();
                    uint32_t mc2 = kernel::heap_small.merge();
                    if (mc1 + mc2 == 0) { perror("Expected heap merge count to be greater than 0"); return false; }
                    //else { printf("%s Merged %u free heap entries\n", DEBUG_INFO, mc1 + mc2); }
                }

                used_after = kernel::heap_large.calc_used() + kernel::heap_small.calc_used();
                if (used_before != used_after) { perror("Memory leak while testing arraylist - PRE:%u bytes POST:%u bytes", used_before, used_after); return false; }
                printf("%s Arraylist test passed\n", DEBUG_OK);
                return true;
            }

            int _test_threads_count;
            int test_threadmain(int argc, char** argv)
            {
                lock();
                printf("Entered test thread\n");

                int counter = 0;
                while (counter < 100000) { counter++; }
                
                unlock();
                _test_threads_count--;
                return 0;
            }

            bool test_scheduler(int iterations)
            {
                uint32_t used_before = kernel::heap_large.calc_used() + kernel::heap_small.calc_used();
                uint32_t used_after  = 0;
                _test_threads_count = 0;

                bool ready = threading::scheduler::is_ready();
                if (!ready) { threading::scheduler::ready(); }
                for (size_t i = 0; i < iterations; i++)
                {

                    lock();
                    asm volatile("cli");
                    _test_threads_count++;
                    threading::thread_t* thread = threading::thread_create("test_thread", 0x10000, test_threadmain, 0, NULL);
                    threading::scheduler::load(thread);
                    threading::scheduler::start(thread);
                    unlock();
                    asm volatile("sti");
                    threading::scheduler::yield();
                }

                int now = 0, last = 0, timer = 0;
                while (_test_threads_count != 0) 
                { 
                    now = hal::pit::seconds();
                    if (now != last)
                    {
                        last = now;
                        timer++;
                        if (timer >= 10) { perror("Failed to terminate %u threads", iterations - _test_threads_count); return false; }
                        printf("%s Waited %us for threads to terminate", timer);
                    }
                    threading::scheduler::yield(); 
                }

                if (!ready) { threading::scheduler::unready(); }
                used_after = kernel::heap_large.calc_used() + kernel::heap_small.calc_used();
                //if (used_before != used_after) { perror("Memory leak while testing scheduler - PRE:%u bytes POST:%u bytes", used_before, used_after); return false; }
                printf("%s Scheduler test passed\n", DEBUG_OK);
                return true;
            }

            bool test_tokenizer(int iterations)
            {
                return false;
            }

            bool test_vbe_iter(hal::vbe_controller* vbe, int w, int h, int test_time)
            {
                std::array<uint32_t> data = std::array<uint32_t>(w * h);
                memset(data.ptr(), 0xFF007F7F, w * h * 4);

                vbe->set_mode(w, h);
                if (vbe->modeinfo().width != w || vbe->modeinfo().height != h) { return false; }
                sleep(test_time);

                vbe->clear(0xFFFFFFFF);
                sleep(test_time);

                vbe->blit(vbe->modeinfo().width / 2, vbe->modeinfo().height / 2, 0xFF000000);
                if (vbe->get_pixel(vbe->modeinfo().width / 2, vbe->modeinfo().height / 2) != 0xFF000000) { return false; }
                sleep(test_time);

                vbe->rect_filled(32, 16, 64, 32, 0xFFFF0000);
                sleep(test_time);

                vbe->putc(vbe->modeinfo().width - std::FONT_DEFAULT.width(), vbe->modeinfo().height - std::FONT_DEFAULT.height(), 'X', std::FONT_DEFAULT, 0xFF00FF00, 0xFF000000);
                sleep(test_time);

                vbe->putstr(0, 0, "Hello world", std::FONT_DEFAULT, 0xFFFFFF00, 0xFF00003F);
                sleep(test_time);

                vbe->copy(0, 0, w, h, data.ptr());
                sleep(test_time);

                data.dispose();
                sleep(test_time);
                return true;
            }

            bool test_vbe(int iterations, int test_time)
            {
                hal::vbe_controller* vbe = hal::devices::vbe;
                int oldw = vbe->modeinfo().width, oldh = vbe->modeinfo().height;
                for (int i = 0; i < iterations; i++)
                {
                    if (!test_vbe_iter(vbe, 640,  480, test_time)) { perror("VBE test failed"); return false; }
                    if (!test_vbe_iter(vbe, 800,  600, test_time)) { perror("VBE test failed"); return false; }
                    if (!test_vbe_iter(vbe, 1024, 768, test_time)) { perror("VBE test failed"); return false; }
                }
                vbe->set_mode(oldw, oldh);
                vbe->clear(0xFF000000);
                if (vbe->modeinfo().width != oldw || vbe->modeinfo().height != oldh) { perror("VBE test failed"); return false; }
                else { printf("%s VBE test passed\n", DEBUG_OK); return true; }
            }
        }
    }
}