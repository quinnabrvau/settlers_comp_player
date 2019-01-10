FROM alpine
COPY all_test.sh /
CMD ["/all_test.sh"]
CMD echo "This is a test." 
